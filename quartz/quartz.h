#ifndef _QUARTZ_
#define _QUARTZ_

#include <glibmm.h>
#include <giomm.h>
#include <sqlite3.h>
#include <iostream>
#include <v8.h>
#include <memory>

namespace Quartz
{
	template <typename type> 
	using RefPtr = std::shared_ptr<type>;

	class AppContext;

	class HttpPackage;
	class HttpRequest;
	class HttpResponse;

	class CacheManager;
	class Cache;

	class DataProvider;
	class SqliteProvider;

}

#include "src/appcontext.h"
#include "src/httppackage.h"
//#include "src/httprequest.h"
//#include "src/httpresponse.h"
#include "src/dataprovider.h"
#include "src/sqliteprovider.h"


#endif