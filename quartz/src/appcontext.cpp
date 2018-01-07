#include "../quartz.h"
#include <string.h>

static v8::Handle<v8::Value> 
AppContext_HttpPost(const v8::Arguments & args)
{
	if(args.Length() == 2)
	{
		Quartz::AppContext * _appcontext_ = (Quartz::AppContext *)v8::External::Unwrap(args.This()->Get(v8::String::New("_appcontext_")));
		v8::String::Utf8Value name(args[0]);
		_appcontext_->AddMethod(Quartz::HTTP_ALLOW_POST,*name, args[1]);
	}
	return v8::Null();
}

static v8::Handle<v8::Value> 
AppContext_HttpGet(const v8::Arguments & args)
{
	if(args.Length() == 2)
	{
		Quartz::AppContext * _appcontext_ = (Quartz::AppContext *)v8::External::Unwrap(args.This()->Get(v8::String::New("_appcontext_")));
		v8::String::Utf8Value name(args[0]);
		_appcontext_->AddMethod(Quartz::HTTP_ALLOW_GET,*name, args[1]);
	}
	return v8::Null();
}

static v8::Handle<v8::Value> 
AppContext_HttpRedirect(const v8::Arguments & args)
{
	Quartz::AppContext * _appcontext_ = (Quartz::AppContext *)v8::External::Unwrap(args.This()->Get(v8::String::New("_appcontext_")));
	return v8::Null();
}


static v8::Handle<v8::Value> 
AppContext_DataQuery(const v8::Arguments & args)
{
	if(args.Length() >= 1)
	{
		Quartz::DataProvider * _provider_ = (Quartz::DataProvider *)v8::External::Unwrap(args.This()->Get(v8::String::New("_provider_")));
		v8::String::Utf8Value query(args[0]);
		v8::Local<v8::Value> params = args[1];
		return _provider_->Query(*query,params);
	} 
	return v8::Null();
}

static v8::Handle<v8::Value> 
AppContext_DataExec(const v8::Arguments & args)
{
	if(args.Length() >= 1)
	{
		Quartz::DataProvider * _provider_ = (Quartz::DataProvider *)v8::External::Unwrap(args.This()->Get(v8::String::New("_provider_")));
		v8::String::Utf8Value query(args[0]);
		v8::Local<v8::Value> params = args[1];
		_provider_->Exec(*query,params);
	} 
	return v8::Null();
}

static v8::Handle<v8::Value> 
AppContext_DataGetLastId(const v8::Arguments & args)
{
	Quartz::AppContext * _appcontext_ = (Quartz::AppContext *)v8::External::Unwrap(args.This()->Get(v8::String::New("_appcontext_")));
}

static v8::Handle<v8::Value> 
AppContext_DataTransaction(const v8::Arguments & args)
{
	if(args.Length() >= 1)
	{
		Quartz::DataProvider * _provider_ = (Quartz::DataProvider *)v8::External::Unwrap(args.This()->Get(v8::String::New("_provider_")));
		return _provider_->Transaction(args[0]);
	} 
	return v8::Null();
}

static v8::Handle<v8::Value> 
AppContext_DataProvider(const v8::Arguments & args)
{
	//TODO: throw bad args
	if(args.Length() == 2)
	{
		Quartz::AppContext * _appcontext_ = (Quartz::AppContext *)v8::External::Unwrap(args.This()->Get(v8::String::New("_appcontext_")));
		if(args[1]->IsObject()) {
			v8::String::Utf8Value provider_name(args[0]);
			v8::Local<v8::Object> config = args[1]->ToObject();
			v8::String::Utf8Value type(config->Get(v8::String::New("type")));
			if(strcmp((*type),"sqlite") == 0)
			{
				_appcontext_->AddDataProvider(
					new Quartz::SQLiteProvider(config->Get(v8::String::New("sqlite"))),*provider_name);
			}
		}
	}
	else
	{
		return v8::Null();
	}
}


static v8::Handle<v8::Value> 
AppContext_GetProvider(const v8::Arguments & args)
{
	Quartz::AppContext * _appcontext_ = (Quartz::AppContext *)v8::External::Unwrap(args.This()->Get(v8::String::New("_appcontext_")));
	Quartz::DataProvider * provider = NULL;
	if(args.Length() >= 1)
	{
		v8::String::Utf8Value name(args[0]);
	 	provider = _appcontext_->GetProvider(*name);
	} else {
		provider = _appcontext_->GetProvider("default");
	}

	if(provider)
	{
		v8::Local<v8::ObjectTemplate> ptmp = v8::ObjectTemplate::New();
		ptmp->Set(v8::String::New("_provider_"),v8::External::New(provider));
		ptmp->Set(v8::String::New("query"),v8::FunctionTemplate::New(AppContext_DataQuery));
		ptmp->Set(v8::String::New("exec"),v8::FunctionTemplate::New(AppContext_DataExec));
		ptmp->Set(v8::String::New("getLastId"),v8::FunctionTemplate::New(AppContext_DataGetLastId));
		ptmp->Set(v8::String::New("transaction"),v8::FunctionTemplate::New(AppContext_DataTransaction));
		return ptmp->NewInstance();
	}
	else
	{
		return v8::Null();
	}
}


static v8::Handle<v8::Value> 
AppContext_Log(const v8::Arguments & args)
{
	if(args.Length() > 0)
	{
		for(int argc = 0; argc < args.Length(); argc ++)
		{
			v8::String::Utf8Value utf8(args[argc]);
			std::cout<<*utf8;
		}
	}
	return v8::Null();
}

Quartz::AppContext::AppContext()
{
	m_global_object = v8::ObjectTemplate::New();
	m_global_object->Set(v8::String::New("_appcontext_"),v8::External::New(this));
	m_global_object->Set(v8::String::New("httpPost"),v8::FunctionTemplate::New(AppContext_HttpPost));
	m_global_object->Set(v8::String::New("httpGet"),v8::FunctionTemplate::New(AppContext_HttpGet));
	m_global_object->Set(v8::String::New("httpRedirect"),v8::FunctionTemplate::New(AppContext_HttpRedirect));

	m_global_object->Set(v8::String::New("dataProvider"),v8::FunctionTemplate::New(AppContext_DataProvider));
	m_global_object->Set(v8::String::New("getProvider"),v8::FunctionTemplate::New(AppContext_GetProvider));

	m_global_object->Set(v8::String::New("log"),v8::FunctionTemplate::New(AppContext_Log));
}

Quartz::AppContext::~AppContext()
{
	
}

Quartz::RefPtr<Quartz::AppContext> Quartz::AppContext::New()
{
	Quartz::RefPtr<Quartz::AppContext> app(new Quartz::AppContext());
	return app;
}

void Quartz::AppContext::AddScript(const std::string & source,const std::string & name)
{
	v8::Persistent<v8::Context> context = v8::Context::New(NULL,m_global_object);
	v8::Context::Scope context_scope(context);
	// Enter the context for compiling and running the hello world script.
	// Create a string containing the JavaScript source code.
	v8::Local<v8::String> v_source =
	    v8::String::New(source.c_str());

	// Compile the source code.
	v8::Local<v8::Script> script =
	    v8::Script::Compile(v_source,v8::String::New(name.c_str()));

	v8::Local<v8::Value> result = script->Run();
}

void Quartz::AppContext::AddMethod(Quartz::HttpAllowMethod allow,const std::string & name,const v8::Handle<v8::Value> & callback)
{
	if(allow == HTTP_ALLOW_POST)
		m_http_post.insert(std::pair<std::string,v8::Handle<v8::Value> >(name,callback));
	else
		m_http_get.insert(std::pair<std::string,v8::Handle<v8::Value> >(name,callback));
}

void Quartz::AppContext::UseSession(const std::string session_id)
{
	m_session_id = session_id;
}

std::string Quartz::AppContext::GetSessionId()
{
	return m_session_id;
}

bool Quartz::AppContext::IsDefined(
	Quartz::HttpAllowMethod allow,
	const std::string & name)
{
	if(allow == Quartz::HTTP_ALLOW_POST)
		return m_http_post.find(name) != m_http_post.end();
	else
		return m_http_get.find(name) != m_http_get.end();
}

static std::string
Quartz_AppContext_SessionId()
{
	Glib::Rand rand;
	const char charset[17] = "abcdef0123456789";
	char newSession[33] = {0,};
	for(int index = 0; index < 32; index ++) 
		newSession[index] = charset[rand.get_int_range(0,15)];
	newSession[32] = 0;
	return newSession;
}

		
v8::Handle<v8::Value> Quartz::AppContext::Invoke(
	Quartz::HttpAllowMethod allow,
	const std::string & name,
	const v8::Handle<v8::Value> & data)
{
	v8::Handle<v8::Function> callback;
	if(allow == Quartz::HTTP_ALLOW_POST)
		callback = v8::Handle<v8::Function>::Cast(m_http_post.find(name)->second);
	else
		callback = v8::Handle<v8::Function>::Cast(m_http_get.find(name)->second);
	std::string session_id = GetSessionId();
	v8::Handle<v8::Object> session;
	if(session_id == "" || (m_session.find(session_id) == m_session.end()))
	{
		do
		{
			session_id = Quartz_AppContext_SessionId();
		}while(m_session.find(session_id) != m_session.end());
		session = v8::Object::New();
		m_session.insert(std::pair<std::string, v8::Handle<v8::Value> >(session_id,session));
		m_session_id = session_id;
	}
	else
	{
		session = v8::Handle<v8::Object>::Cast(m_session.find(session_id)->second);
	}
	v8::Handle<v8::Value> args[1];
	args[0] = data;
	return callback->Call(session,1,args);
}


void Quartz::AppContext::AddDataProvider(Quartz::DataProvider * provider, const std::string & name)
{
	m_data_provider.insert(std::pair<std::string, Quartz::DataProvider *>(name,provider));
}

Quartz::DataProvider * Quartz::AppContext::GetProvider(const std::string & name)
{
	std::map<std::string,Quartz::DataProvider* >:: iterator it = m_data_provider.find(name);
	if(it != m_data_provider.end())
		return it->second;
	else
		return NULL;
}

void Quartz::AppContext::AddDirScript(const std::string & dir_path)
{
	if(Glib::file_test(dir_path,Glib::FILE_TEST_IS_DIR|Glib::FILE_TEST_EXISTS)) 
	{
		Glib::Dir dir(dir_path);
		for(Glib::Dir::iterator it = dir.begin(); it != dir.end(); it++)
		{
			std::string item_path = Glib::build_filename(dir_path,*it);
			if(Glib::file_test(item_path,Glib::FILE_TEST_IS_DIR|Glib::FILE_TEST_EXISTS) && Glib::str_has_suffix(item_path,".js"))
				AddScript(Glib::file_get_contents(item_path),item_path);
			else
				AddDirScript(item_path);
		}
	}

}