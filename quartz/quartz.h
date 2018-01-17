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

#ifndef QUARTZ_H_
#define QUARTZ_H_

#include <glibmm.h>
#include <giomm.h>


namespace Quartz
{
  template <typename T_RefPtr>
  using RefPtr = std::shared_ptr<T_RefPtr>;

  namespace Web {
    enum HttpMethod
    {
      HTTP_METHOD_GET,
      HTTP_METHOD_HEAD,
      HTTP_METHOD_POST,
      HTTP_METHOD_INVALID
    };

    enum HttpResponseCode
    {
      HTTP_RESPONSE_OK = 200,
      HTTP_RESPONSE_BAD_REQUEST = 400,
      HTTP_RESPONSE_UNAUTHORIZED = 401,
      HTTP_RESPONSE_FORBIDDEN = 403,
      HTTP_RESPONSE_NOT_FOUND = 404,
      HTTP_RESPONSE_METHOD_NOT_ALLOWED = 405,
      HTTP_RESPONSE_LENGTH_REQUIRED = 411,
      HTTP_RESPONSE_TO_MANY_REQUESTS = 429,
      HTTP_RESPONSE_INTERNAL_ERROR_SERVER = 500,
      HTTP_RESPONSE_NOT_IMPLEMENTED = 501
    };

    enum HttpCompressionType
    {
      HTTP_COMPRESSION_NONE,
      HTTP_COMPRESSION_GZIP,
      HTTP_COMPRESSION_DEFLATE
    };

    class HttpPackage;
    class HttpRequest;
    class HttpResponse;
  }

  namespace JSON {
    class Base;
    class Reader;
    class Builder;
    class Node;
  }

  class Application;
  class Session;
  class Context;
  class Cache;
  class Controller;
  class Result;
}

#include "web/httppackage.h"
#include "web/httprequest.h"
#include "web/httpresponse.h"

#include "json/reader.h"

/*#include "json/serializablebase.h"
#include "json/serializable.h"
#include "json/model.h"
#include "json/array.h"
*/

#include "application.h"
#include "session.h"
#include "cache.h"

#endif /* QUARTZ_H_ */
