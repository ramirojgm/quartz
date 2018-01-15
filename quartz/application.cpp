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

Quartz::Application::Application()
{
  m_service = Gio::ThreadedSocketService::create(1024);
  m_main_loop = Glib::MainLoop::create();

  g_mutex_init(&m_controller_mutex);
  g_mutex_init(&m_cache_mutex);

  m_service->signal_run()
      .connect(sigc::mem_fun(*this,&Application::on_service_run));
}

Quartz::Application::~Application()
{
  g_mutex_clear(&m_controller_mutex);
  g_mutex_clear(&m_cache_mutex);

 /* for(std::map<Glib::ustring,Controller*>::iterator it = m_controller.begin();
      it != m_controller.end();
      it++)
    {
      delete it->second;
      it->second = 0;
    }
*/

  for(std::map<Glib::ustring,Cache*>::iterator it = m_cache.begin();
      it != m_cache.end();
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

template<typename T_Controller>
void
Quartz::Application::map(const Glib::ustring & vpath)
{

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

    gsize written = 0;

    if(request->get_method() == Web::HTTP_METHOD_GET)
      {
	//Search for a file in the server
	Glib::ustring query = request->get_query();
	if(Glib::str_has_suffix(query,"/"))
	  query = Glib::build_filename(query,"index.htm");

	g_mutex_lock(&m_cache_mutex);
	std::map<Glib::ustring,Cache*>::iterator it;
	if((it = m_cache.find(query)) != m_cache.end())
	  {
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
	else
	  {
	    response->set_response_code(Web::HTTP_RESPONSE_NOT_FOUND);
	    response->write(out);
	  }
	g_mutex_unlock(&m_cache_mutex);
      }

    return true;
}
