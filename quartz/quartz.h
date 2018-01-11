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
  template <typename refptr_t>
  using RefPtr = std::shared_ptr<refptr_t>;

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

    class HttpService;
    class HttpPackage;
    class HttpRequest;
    class HttpResponse;
    class HttpSession;
    class HttpSettings;
  }

  namespace Data {
    class SerializableBase;
    class Model;
    class Array;
    template<typename value_t> class Serializable;
  }

  namespace Application {
    class Application;
    class Controller;
    class Action;
  }
}

#include "data/serializablebase.h"
#include "data/serializable.h"
#include "data/model.h"
#include "data/array.h"

#endif /* QUARTZ_H_ */
