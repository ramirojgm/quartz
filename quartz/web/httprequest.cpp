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

#include "../quartz.h"

Quartz::Web::HttpRequest::HttpRequest()
{
  m_method = HTTP_METHOD_GET;
  m_query = "/";
  m_query_param = "";
}

Quartz::Web::HttpRequest::~HttpRequest()
{

}

Quartz::RefPtr<Quartz::Web::HttpRequest>
Quartz::Web::HttpRequest::create()
{
  Quartz::RefPtr<Quartz::Web::HttpRequest> request(new HttpRequest());
  return request;
}

Quartz::RefPtr<Quartz::Web::HttpRequest>
Quartz::Web::HttpRequest::create(
    const Glib::RefPtr<Gio::InputStream> & stream)
{
  Quartz::RefPtr<Quartz::Web::HttpRequest> request(new HttpRequest());
  Glib::RefPtr<Gio::DataInputStream> data_stream = HttpPackage::buffer_stream(stream);
  request->read(data_stream);
  return request;
}

void
Quartz::Web::HttpRequest::read(
    const Glib::RefPtr<Gio::DataInputStream> & stream)
{
  gsize line_length;
  std::string line;
  std::string::size_type sep_pos = 0;

  stream->read_line_utf8(line,line_length);
  if(line_length > 0)
    {

      if((sep_pos = line.find(" ")) != std::string::npos)
	{
	  std::string method(line.substr(0,sep_pos));
	  std::string query(line.substr(sep_pos + 1));
	  if((sep_pos = query.find(" ")) != std::string::npos)
	    {
	      std::string new_query(query.substr(0,sep_pos));
	      if((sep_pos = new_query.find("?")) != std::string::npos)
		{
		  m_query = new_query.substr(0,sep_pos);
		  m_query_param = new_query.substr(sep_pos + 1);
		}
	      else
		{
		  m_query = new_query;
		  m_query_param = "";
		}
	    }
	  else
	    {
	      m_query = "";
	      m_query_param = "";
	    }

	  if(method == "GET")
	    m_method = HTTP_METHOD_GET;
	  else if(method == "POST")
	    m_method = HTTP_METHOD_POST;
	  else if(method == "HEAD")
	    m_method = HTTP_METHOD_HEAD;
	  else
	    m_method = HTTP_METHOD_INVALID;

	  HttpPackage::read_attributes(stream);
	}
    }
}

Quartz::Web::HttpMethod
Quartz::Web::HttpRequest::get_method()
{
  return m_method;
}

Glib::ustring
Quartz::Web::HttpRequest::get_query()
{
  return m_query;
}

Glib::ustring
Quartz::Web::HttpRequest::get_query_param()
{
  return m_query_param;
}
