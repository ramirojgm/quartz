#include "../quartz.h"

Quartz::SQLiteProvider::SQLiteProvider(const v8::Handle<v8::Value> & config)
{
	v8::Local<v8::Object> objectConfig = config->ToObject();
	if(objectConfig->Has(v8::String::New("path")))
	{
		v8::String::Utf8Value path(objectConfig->Get(v8::String::New("path")));
		sqlite3_open(*path,&(this->m_db));
	}
	else
	{
		sqlite3_open(":memory:",&(this->m_db));
	}
}

Quartz::SQLiteProvider::~SQLiteProvider()
{
	sqlite3_close(this->m_db);
}


v8::Handle<v8::Object> Quartz::SQLiteProvider::Query(const std::string & query,const v8::Handle<v8::Value> & data)
{
	sqlite3_stmt * stmt = NULL;
	if(sqlite3_prepare(m_db,query.c_str(),-1,&stmt,NULL) == SQLITE_OK)
	{
		if(data->IsObject()) 
		{
			v8::Local<v8::Object> dataObject = data->ToObject();
			int param_count = sqlite3_bind_parameter_count(stmt);
			for(int param_index = 1;param_index <= param_count;param_index ++)
			{
				v8::Local<v8::String> param_name = v8::String::New(sqlite3_bind_parameter_name(stmt,param_index) + 1);
				if(dataObject->Has(param_name)) {
					v8::Local<v8::Value> param_value = dataObject->Get(param_name);

					if(param_value->IsBoolean())
					{
						sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
					}
					else if(param_value->IsNumber())
					{
						sqlite3_bind_double(stmt,param_index,param_value->NumberValue());
					}
					else if(param_value->IsUint32())
					{
						sqlite3_bind_int(stmt,param_index,param_value->Uint32Value());
					}
					else if(param_value->IsInt32())
					{
						sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
					}
					else if(param_value->IsDate())
					{
						double millisSinceEpoch = v8::Date::Cast(*param_value)->NumberValue(); 
						std::time_t t = static_cast<time_t>(millisSinceEpoch/1000);
					    struct tm* ltime = localtime(&t);
						char * date_string = g_strdup_printf("%d-%d-%d %d:%d:%d",
												ltime->tm_year,
												ltime->tm_mon + 1,
												ltime->tm_mday,
												ltime->tm_hour,
												ltime->tm_min,
												ltime->tm_sec);
						sqlite3_bind_text(stmt,param_index,date_string,-1,g_free);
					}
					else
					{
						v8::String::Utf8Value str(param_value);
						sqlite3_bind_text(stmt,param_index,*str,-1,NULL);
					}
				}
			}
		} 
		else if (data->IsArray())
		{
			int param_count = sqlite3_bind_parameter_count(stmt);
			v8::Local<v8::Object> dataArray = data->ToObject();
			for(int param_index = 1;param_index <= param_count;param_index ++)
			{
				v8::Local<v8::Value> param_value = dataArray->Get(param_index);

				if(param_value->IsBoolean())
				{
					sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
				}
				else if(param_value->IsNumber())
				{
					sqlite3_bind_double(stmt,param_index,param_value->NumberValue());
				}
				else if(param_value->IsUint32())
				{
					sqlite3_bind_int(stmt,param_index,param_value->Uint32Value());
				}
				else if(param_value->IsInt32())
				{
					sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
				}
				else if(param_value->IsDate())
				{
					double millisSinceEpoch = v8::Date::Cast(*param_value)->NumberValue(); 
					std::time_t t = static_cast<time_t>(millisSinceEpoch/1000);
				    struct tm* ltime = localtime(&t);
					char * date_string = g_strdup_printf("%d-%d-%d %d:%d:%d",
											ltime->tm_year,
											ltime->tm_mon + 1,
											ltime->tm_mday,
											ltime->tm_hour,
											ltime->tm_min,
											ltime->tm_sec);
					sqlite3_bind_text(stmt,param_index,date_string,-1,g_free);
				}
				else
				{
					v8::String::Utf8Value str(param_value);
					sqlite3_bind_text(stmt,param_index,*str,-1,NULL);
				}
			}
		}
		v8::Local<v8::Object> result = v8::Array::New();
		for(int row_index = 0;sqlite3_step(stmt) == SQLITE_ROW;row_index ++)
		{
			v8::Local<v8::Object> row = v8::Object::New();
			int col_count = sqlite3_column_count(stmt);
			for(int col_index = 0; col_index < col_count; col_index ++)
			{
				v8::Local<v8::Value> col_name = v8::String::New(sqlite3_column_name(stmt,col_index));
				int col_type = sqlite3_column_type(stmt,col_index);
				switch(col_type)
				{
				case SQLITE_INTEGER:
				row->Set(col_name,v8::Int32::New(sqlite3_column_int(stmt,col_index)));
				break;
				case SQLITE_FLOAT:
				row->Set(col_name,v8::Number::New(sqlite3_column_double(stmt,col_index)));
				break;
				case SQLITE_NULL:
				row->Set(col_name,v8::Null());
				break;
				case SQLITE_TEXT:
				row->Set(col_name,v8::String::New((char*)sqlite3_column_text(stmt,col_index),sqlite3_column_bytes(stmt,col_index)));
				break;
				}
			}
			result->Set(row_index,row);
		}
		sqlite3_finalize(stmt);
		return result;
	} 
	else 
	{
		v8::ThrowException(v8::String::New(sqlite3_errmsg(m_db)));
	}
	return v8::Array::New();
}

v8::Handle<v8::Value> Quartz::SQLiteProvider::Transaction(v8::Handle<v8::Value> callback)
{

}

void Quartz::SQLiteProvider::Exec(const std::string & sql,const v8::Handle<v8::Value> & data)
{
	sqlite3_stmt * stmt = NULL;
	if(sqlite3_prepare(m_db,sql.c_str(),-1,&stmt,NULL) == SQLITE_OK)
	{
		if(data->IsObject()) 
		{
			v8::Local<v8::Object> dataObject = data->ToObject();
			int param_count = sqlite3_bind_parameter_count(stmt);
			for(int param_index = 1;param_index <= param_count;param_index ++)
			{
				v8::Local<v8::String> param_name = v8::String::New(sqlite3_bind_parameter_name(stmt,param_index) + 1);
				if(dataObject->Has(param_name)) {
					v8::Local<v8::Value> param_value = dataObject->Get(param_name);

					if(param_value->IsBoolean())
					{
						sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
					}
					else if(param_value->IsNumber())
					{
						sqlite3_bind_double(stmt,param_index,param_value->NumberValue());
					}
					else if(param_value->IsUint32())
					{
						sqlite3_bind_int(stmt,param_index,param_value->Uint32Value());
					}
					else if(param_value->IsInt32())
					{
						sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
					}
					else if(param_value->IsDate())
					{
						double millisSinceEpoch = v8::Date::Cast(*param_value)->NumberValue(); 
						std::time_t t = static_cast<time_t>(millisSinceEpoch/1000);
					    struct tm* ltime = localtime(&t);
						char * date_string = g_strdup_printf("%d-%d-%d %d:%d:%d",
												ltime->tm_year + 1900,
												ltime->tm_mon + 1,
												ltime->tm_mday,
												ltime->tm_hour,
												ltime->tm_min,
												ltime->tm_sec);
						sqlite3_bind_text(stmt,param_index,date_string,-1,g_free);
					}
					else
					{
						v8::String::Utf8Value str(param_value);
						sqlite3_bind_text(stmt,param_index,g_strdup(*str),-1,g_free);
					}
				}
			}
		} 
		else if (data->IsArray())
		{
			int param_count = sqlite3_bind_parameter_count(stmt);
			v8::Local<v8::Object> dataArray = data->ToObject();
			for(int param_index = 1;param_index <= param_count;param_index ++)
			{
				v8::Local<v8::Value> param_value = dataArray->Get(param_index);

				if(param_value->IsBoolean())
				{
					sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
				}
				else if(param_value->IsNumber())
				{
					sqlite3_bind_double(stmt,param_index,param_value->NumberValue());
				}
				else if(param_value->IsUint32())
				{
					sqlite3_bind_int(stmt,param_index,param_value->Uint32Value());
				}
				else if(param_value->IsInt32())
				{
					sqlite3_bind_int(stmt,param_index,param_value->Int32Value());
				}
				else if(param_value->IsDate())
				{
					double millisSinceEpoch = v8::Date::Cast(*param_value)->NumberValue(); 
					std::time_t t = static_cast<time_t>(millisSinceEpoch/1000);
				    struct tm* ltime = localtime(&t);
					char * date_string = g_strdup_printf("%d-%d-%d %d:%d:%d",
											ltime->tm_year + 1900,
											ltime->tm_mon + 1,
											ltime->tm_mday,
											ltime->tm_hour,
											ltime->tm_min,
											ltime->tm_sec);
					sqlite3_bind_text(stmt,param_index,date_string,-1,g_free);
				}
				else
				{
					v8::String::Utf8Value str(param_value);
					sqlite3_bind_text(stmt,param_index,g_strdup(*str),-1,g_free);
				}
			}
		}

		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		v8::ThrowException(v8::String::New(sqlite3_errmsg(m_db)));
	}
}

int Quartz::SQLiteProvider::GetLastId()
{

}