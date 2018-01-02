#ifndef QUARTZ_DATA_PROVIDER_
#define QUARTZ_DATA_PROVIDER_

namespace Quartz
{
	class DataProvider
	{
	protected:
		DataProvider();

	public:
		virtual ~DataProvider();

		virtual v8::Handle<v8::Object> Query(const std::string & query,const v8::Handle<v8::Value> & data) = 0;

		virtual v8::Handle<v8::Value> Transaction(v8::Handle<v8::Value> callback) = 0;

		virtual void Exec(const std::string & sql,const v8::Handle<v8::Value> & data) = 0;

		virtual int GetLastId() = 0;
	};
}

#endif
