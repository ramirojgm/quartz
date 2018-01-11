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

#ifndef QUARTZ_WEB_HTTPREQUEST_H_
#define QUARTZ_WEB_HTTPREQUEST_H_

namespace Quartz
{
  namespace Web
  {
    class HttpRequest: public HttpPackage
    {
    private:
      HttpRequest();

    public:
      virtual ~HttpRequest();

      static Quartz::RefPtr<HttpRequest> create();
      static Quartz::RefPtr<HttpRequest> create(const Glib::RefPtr<Gio::InputStream> & stream);

      void read(const Glib::RefPtr<Gio::DataInputStream> & stream);

    };
  }
}

#endif /* QUARTZ_WEB_HTTPREQUEST_H_ */
