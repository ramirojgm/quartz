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

Quartz::Web::HttpResponse::HttpResponse()
{
  m_response_code = HTTP_RESPONSE_OK;
}

Quartz::Web::HttpResponse::~HttpResponse()
{

}

Quartz::RefPtr<Quartz::Web::HttpResponse>
Quartz::Web::HttpResponse::create()
{
  Quartz::RefPtr<Quartz::Web::HttpResponse> response(new HttpResponse());
  return response;
}

void
Quartz::Web::HttpResponse::write(
    const Glib::RefPtr<Gio::DataOutputStream> & stream)
{
  Glib::ustring response_text;
  switch(m_response_code)
  {
    case HTTP_RESPONSE_OK:
      response_text = "OK";
      break;
    case HTTP_RESPONSE_BAD_REQUEST:
      response_text = "Bad Request";
      break;
    case HTTP_RESPONSE_FORBIDDEN:
      response_text = "Forbidden";
      break;
    case HTTP_RESPONSE_LENGTH_REQUIRED:
      response_text = "Length Required";
      break;
    case HTTP_RESPONSE_METHOD_NOT_ALLOWED:
      response_text = "Method Not Allowed";
      break;
    case HTTP_RESPONSE_INTERNAL_ERROR_SERVER:
      response_text = "Internal Error Server";
      break;
    case HTTP_RESPONSE_NOT_FOUND:
      response_text = "Not Found";
      break;
    case HTTP_RESPONSE_NOT_IMPLEMENTED:
      response_text = "Not Implemented";
      break;
    case HTTP_RESPONSE_TO_MANY_REQUESTS:
      response_text = "To Many Requests";
      break;
    case HTTP_RESPONSE_UNAUTHORIZED:
      response_text = "Unauthorized";
      break;
    default:
      response_text = "OK";
  }

  stream->write(Glib::ustring::compose("HTTP/1.1 %1 %2\r\n",(gint)m_response_code,response_text));
  HttpPackage::write_attributes(stream);
}

Quartz::Web::HttpResponseCode
Quartz::Web::HttpResponse::get_response_code()
{
  return m_response_code;
}

void
Quartz::Web::HttpResponse::set_response_code(Quartz::Web::HttpResponseCode code)
{
  m_response_code = code;
}
