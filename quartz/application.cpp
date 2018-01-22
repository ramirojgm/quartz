/*
	Copyright (C) 2018 Ramiro Jose Garcia Moraga

	This file is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with the this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quartz.h"
#include <iostream>

Quartz::Application::Application()
{
  m_service = Gio::ThreadedSocketService::create(1024);
  m_main_loop = Glib::MainLoop::create();

  g_mutex_init(&m_controller_mutex);
  g_mutex_init(&m_cache_mutex);
  g_mutex_init(&m_session_mutex);

  m_service->signal_run()
      .connect(sigc::mem_fun(*this,&Application::on_service_run));
}

Quartz::Application::~Application()
{
  g_mutex_clear(&m_controller_mutex);
  g_mutex_clear(&m_cache_mutex);
  g_mutex_clear(&m_session_mutex);

  for(std::map<Glib::ustring,Controller*>::iterator it = m_controller.begin();
      it != m_controller.end();
      it++)
    {
      delete it->second;
      it->second = 0;
    }

  for(std::map<Glib::ustring,Cache*>::iterator it = m_cache.begin();
      it != m_cache.end();
      it++)
    {
      delete it->second;
      it->second = 0;
    }

  for(std::map<Glib::ustring,Session*>::iterator it = m_session.begin();
      it != m_session.end();
      it++)
    {
      delete it->second;
      it->second = 0;
    }
}

void
Quartz::Application::add_inet_port(guint16 inet_port)
{
  m_service->add_inet_port(inet_port);
}

void
Quartz::Application::map(
    const Glib::ustring & vpath,
    const Glib::ustring & path)
{
  Glib::Dir dir(path);
  for(Glib::DirIterator it = dir.begin(); it != dir.end(); it++)
    {
      Glib::ustring new_path = Glib::build_filename(path,*it);
      Glib::ustring new_vpath = Glib::build_filename(vpath,*it);
      if(Glib::file_test(new_path,Glib::FILE_TEST_IS_DIR))
	{
	  //Sub Directory
	  map(new_vpath,new_path);
	}
      else
	{
	  //File
	  g_mutex_lock(&m_cache_mutex);
	  m_cache[new_vpath] = new Cache(new_path);
	  g_mutex_unlock(&m_cache_mutex);
	}
    }
  dir.close();
}

void
Quartz::Application::map(const Glib::ustring & vpath,Controller * controller)
{
  g_mutex_lock(&m_controller_mutex);
  m_controller[vpath] = controller;
  g_mutex_unlock(&m_controller_mutex);
}

void
Quartz::Application::run()
{
  m_service->start();
  m_main_loop->run();
}

void
Quartz::Application::quit()
{
  m_service->stop();
  m_main_loop->quit();
}

bool
Quartz::Application::on_service_run(
    const Glib::RefPtr<Gio::SocketConnection> &  socket_connection,
    const Glib::RefPtr<Glib::Object> & source)
{
    Glib::DateTime cur_time = Glib::DateTime::create_now_local();

    Glib::RefPtr<Gio::InputStream> in = socket_connection->get_input_stream();
    Glib::RefPtr<Gio::DataOutputStream> out = Gio::DataOutputStream::create(socket_connection->get_output_stream());

    Quartz::RefPtr<Quartz::Web::HttpRequest>
    request = Quartz::Web::HttpRequest::create(in);

    Quartz::RefPtr<Quartz::Web::HttpResponse>
    response = Quartz::Web::HttpResponse::create();
    response->set("Date",cur_time.format("%a, %e %b %Y %T %Z"));
    response->set("Server","Quartz/1.0.0");
    response->set("Content-Length",0l);
    response->set("Connection","close");

    bool not_found = true;
    gsize written = 0;

    if(request->get_method() == Web::HTTP_METHOD_GET || request->get_method() == Web::HTTP_METHOD_POST)
      {
	if(request->get_method() == Web::HTTP_METHOD_GET)
	  {
	    //STATIC CONTENT
	    Glib::ustring query = request->get_query();
	    if(Glib::str_has_suffix(query,"/"))
	      query = Glib::build_filename(query,"index.htm");

	    g_mutex_lock(&m_cache_mutex);
	    std::map<Glib::ustring,Cache*>::iterator it;
	    if((it = m_cache.find(query)) != m_cache.end())
	      {
		not_found = false;
		Glib::ustring encoding = "";
		if(request->has("Accept-Encoding"))
		  encoding = request->get_string("Accept-Encoding");

		Glib::ustring mime_type;
		Glib::ustring modification_time;
		const guint8 * content_data;
		gsize content_size;

		if(encoding.find("deflate") != std::string::npos)
		  {
		    response->set("Content-Encoding","deflate");
		    content_data = it->second->get_data(
			Web::HTTP_COMPRESSION_DEFLATE,
			mime_type,
			modification_time,
			content_size);
		  }
		else if(encoding.find("gzip") != std::string::npos)
		  {
		    response->set("Content-Encoding","gzip");
		    content_data = it->second->get_data(
			Web::HTTP_COMPRESSION_GZIP,
			mime_type,
			modification_time,
			content_size);
		  }
		else
		  {
		    content_data = it->second->get_data(
			Web::HTTP_COMPRESSION_NONE,
			mime_type,
			modification_time,
			content_size);
		  }

		response->set("Content-Type",mime_type);
		response->set("Last-Modified",modification_time);
		response->set("Content-Length",(gint64)content_size);

		response->write(out);
		if(content_size > 0)
		    out->write_all(content_data,content_size,written);
	      }
	    g_mutex_unlock(&m_cache_mutex);
	  }

	if (not_found)
	  {
	    //DYNAMIC CONTENT
	    Glib::ustring query_base = request->get_query().substr(1);
	    if(query_base.empty())
	      query_base = "default";

	    Glib::ustring session_id = "";
	    Glib::ustring controller_name = "";
	    Glib::ustring action_name = "";
	    bool volatile_sesion = false;
	    Session * session = NULL;

	    if(query_base != "")
	      {
		std::string::size_type npos = 0;
		if((npos = query_base.find("/")) != std::string::npos)
		  {
		    controller_name = query_base.substr(0,npos);
		    action_name = query_base.substr(npos + 1);
		  }
		else
		  {
		    controller_name = query_base;
		    action_name = "default";
		  }
	      }


	    if(request->has("Cookie"))
	      {
		Glib::ustring cookie = request->get_string("Cookie");
		std::string::size_type npos_cookie = 0;
		std::string::size_type npos_cookie_stop = 0;

		if((npos_cookie = cookie.find("quartz=")) != std::string::npos)
		  {
		    npos_cookie_stop = cookie.find(";",npos_cookie);
		    if(npos_cookie_stop != std::string::npos)
		      session_id = cookie.substr(npos_cookie + 7,npos_cookie_stop - (npos_cookie + 7));
		    else
		      session_id = cookie.substr(npos_cookie + 7);
		  }
	      }

	    g_mutex_lock(&m_session_mutex);
	    if(session_id.empty() || m_session.find(session_id) == m_session.end())
	      {
		if(request->get_method() == Web::HTTP_METHOD_POST)
		  {
		    volatile_sesion = false;
		    session_id = "";
		    gchar hex_val[] = "0123456789abcdef";
		    gchar hex[21] = {0,};

		    while(m_session.find(session_id) != m_session.end())
		      {
			Glib::Rand rand;
			for(gint c = 0;c <= 19;c ++)
			  {
			    if(c == 4||c == 12)
			      hex[c] = '-';
			    else
			      hex[c] = hex_val[rand.get_int_range(0,15)];
			  }
			hex[20] = 0;
			session_id = Glib::ustring(hex);
		      }

		    response->set("Set-Cookie",Glib::ustring::compose("quartz=%1; HttpOnly",session_id));
		    session = new Session();
		    m_session[session_id] = session;
		  }
		else
		  {
		    volatile_sesion = true;
		    session = new Session();
		  }
	      }
	    else
	      {
		session = m_session[session_id];
	      }

	    g_mutex_unlock(&m_session_mutex);

	    if(request->get_method() == Web::HTTP_METHOD_POST && !request->has("Content-Length"))
	      {
		not_found = false;
		response->set_response_code(Web::HTTP_RESPONSE_LENGTH_REQUIRED);
		response->write(out);
	      }
	    else
	      {
		g_mutex_lock(&m_controller_mutex);
		auto it_controller = m_controller.find(controller_name);
		if(it_controller != m_controller.end())
		  {
		    not_found = false;
		    Controller * controller = it_controller->second;
		    if(controller->has(action_name))
		      {
			//Invoke Action
			Glib::ustring body_content = "";
			if(request->get_method() == Web::HTTP_METHOD_POST)
			  {
			    gint body_length = request->get_int("Content-Length");
			    if(body_length > 0)
			      {
				gsize read_count;
				gpointer body_data = g_malloc(body_length);
				in->read_all(body_data,body_length,read_count);
				body_content = Glib::ustring((gchar*)body_data,body_length);
				g_free(body_data);
			      }
			  }
			else
			  {
			    body_content = request->get_query_param();
			  }

			Quartz::RefPtr<Context> context(new Context(*session,body_content));
			try
			{
			    Quartz::RefPtr<Quartz::Result>
			    result = controller->invoke(action_name,context);
			    result->run(response);
			    Glib::ustring content = result->get_content();
			    response->set("Content-Length",(gint64)content.length());
			    response->write(out);
			    if(!content.empty())
				out->write_all(content,written);
			}
			catch(std::exception * ex)
			{
			    Glib::ustring message = ex->what();
			    response->set_response_code(Web::HTTP_RESPONSE_INTERNAL_ERROR_SERVER);
			    response->set("Content-Length",(gint64)message.length());
			    response->write(out);
			    out->write(message);
			    delete ex;
			}
		      }
		    else
		      {
			//NOT FOUND METHOD
			response->set_response_code(Web::HTTP_RESPONSE_NOT_FOUND);
			response->write(out);
		      }
		  }
		else
		  {
		    //NOT FOUND CONTROLLER
		    response->set_response_code(Web::HTTP_RESPONSE_NOT_FOUND);
		    response->write(out);
		  }

		if(volatile_sesion)
		  delete session;

		g_mutex_unlock(&m_controller_mutex);
	      }
	  }

	if(not_found)
	  {
	    //NOT FOUND
	    response->set_response_code(Web::HTTP_RESPONSE_NOT_FOUND);
	    response->write(out);
	  }
      }
    else
      {
	//NOT METHOD ALLOWED
	response->set_response_code(Web::HTTP_RESPONSE_METHOD_NOT_ALLOWED);
	response->write(out);
      }
    return true;
}
