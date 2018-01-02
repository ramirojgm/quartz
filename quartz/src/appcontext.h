#ifndef QUARTZ_APP_CONTEXT_
#define QUARTZ_APP_CONTEXT_


namespace Quartz
{
	enum HttpAllowMethod
	{
		HTTP_ALLOW_POST,
		HTTP_ALLOW_GET
	};

	class AppContext
	{
	private:
		AppContext();

	public:
		static RefPtr<AppContext>  New();

		void AddScript(const std::string & script,const std::string & name);

		void AddDirScript(const std::string & dir);

		void AddDataProvider(DataProvider * provider, const std::string & name);

		DataProvider * GetProvider(const std::string & name);

		void AddMethod(HttpAllowMethod allow, const std::string & name,const v8::Handle<v8::Value> & callback);

		bool IsDefined(HttpAllowMethod allow, const std::string & name);

		void UseSession(const std::string session_id);

		std::string GetSessionId();

		v8::Handle<v8::Value> Invoke(HttpAllowMethod allow,const std::string & name,const v8::Handle<v8::Value> & data);

		virtual ~AppContext();

	private:
		//v8
		v8::Handle<v8::ObjectTemplate> 	m_global_object;

		//data
		std::map<std::string,Quartz::DataProvider*> m_data_provider;

		//service 
		std::string m_session_id;
		std::map<std::string,v8::Handle<v8::Value> > m_http_post;
		std::map<std::string,v8::Handle<v8::Value> > m_http_get;
		std::map<std::string,v8::Handle<v8::Value> > m_session;
	};
}

#endif