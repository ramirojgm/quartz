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

#ifndef QUARTZ_WEB_HTTPRESPONSE_H_
#define QUARTZ_WEB_HTTPRESPONSE_H_

namespace Quartz
{
  namespace Web
  {
    class HttpResponse: public HttpPackage
    {
    private:
      HttpResponse();

    public:
      virtual ~HttpResponse();

      static Quartz::RefPtr<HttpResponse> create();

      void write(const Glib::RefPtr<Gio::DataOutputStream> & stream);

      void set_response_code(HttpResponseCode code);

      HttpResponseCode get_response_code(void);

    private:
      HttpResponseCode m_response_code;
    };
  }
}

#endif /* QUARTZ_WEB_HTTPRESPONSE_H_ */
