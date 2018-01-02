#ifndef QUARTZ_SQLITE_PROVIDER_
#define QUARTZ_SQLITE_PROVIDER_

namespace Quartz
{
	class SQLiteProvider:public DataProvider
	{

	public:
		SQLiteProvider(const v8::Handle<v8::Value> & config);

		virtual ~SQLiteProvider();

		static RefPtr<DataProvider> New(const v8::Handle<v8::Value> & config);

		v8::Handle<v8::Object> Query(const std::string & query,const v8::Handle<v8::Value> & data);

		v8::Handle<v8::Value> Transaction(v8::Handle<v8::Value> callback);

		void Exec(const std::string & sql,const v8::Handle<v8::Value> & data);

		int GetLastId();

	private:
		sqlite3 * m_db = NULL;
	};
}

#endif

