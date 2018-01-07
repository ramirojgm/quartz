/*
	Copyright (C) 2017 Ramiro Jose Garcia Moraga

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

Quartz::HttpRequest::HttpRequest()
{

}

Quartz::HttpRequest::~HttpRequest()
{

}

Quartz::RefPtr<Quartz::HttpRequest> Quartz::HttpRequest::create()
{
	Quartz::RefPtr<Quartz::HttpRequest> request(new Quartz::HttpRequest());
	return request;
}

void Quartz::HttpRequest::ReadFromStream(
	const Glib::RefPtr<Gio::DataInputStream> & stream,
	const Glib::RefPtr<Gio::Cancellable> & cancellable)
{
	std::string line;
	gsize line_length = 0;

	stream->read_line_utf8(line,cancellable,line_length);
	if(line_length > 0)
	{
		std::string method;

		std::stringstream buffer(line);
		buffer>>method>>m_query>>m_version;

		if(method == "GET")
			m_method = Quartz::HTTP_METHOD_GET;
		else if(method == "POST")
			m_method = Quartz::HTTP_METHOD_POST;
		else
			m_method = Quartz::HTTP_METHOD_INVALID;

		Quartz::HttpPackage::ReadFromStream(stream,cancellable);
	}
}

Quartz::HttpMethod	
Quartz::HttpRequest::get_method()
{
	return m_method;
}

std::string Quartz::HttpRequest::get_query()
{
	return m_query;
}

double Quartz::HttpRequest::get_version()
{
	return m_version;
}
