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

#ifndef QUARTZ_HTTP_REQUEST_
#define QUARTZ_HTTP_REQUEST_

namespace Quartz
{
	enum HttpMethod
	{
		HTTP_METHOD_GET = 0,
		HTTP_METHOD_POST = 1,
		HTTP_METHOD_INVALID = 2
	};

	class HttpRequest: public HttpPackage {
	private:
		HttpRequest();

	public:

		virtual ~HttpRequest();

		static Quartz::RefPtr<HttpRequest> create();

		void ReadFromStream(const Glib::RefPtr<Gio::DataInputStream> & stream,const Glib::RefPtr<Gio::Cancellable> & cancellable);

		HttpMethod	get_method();

		std::string get_query();

		double		get_version();

	private:
		HttpMethod	m_method;
		std::string m_query;
		double		m_version;
	};
}
#endif
