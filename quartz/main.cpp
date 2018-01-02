#include "quartz.h"

static std::string site_dir;
static std::string data_dir;
static std::string src_dir;
static std::string content_dir;
static Quartz::RefPtr<Quartz::AppContext> context;

int main(int argc,char ** argv)
{
	site_dir = Glib::get_current_dir();
	if(argc > 1)
		site_dir = argv[1];

	data_dir = Glib::build_filename(site_dir,"js/data/");
	src_dir = Glib::build_filename(site_dir,"js/src/");
	content_dir = Glib::build_filename(site_dir,"content/");

	v8::V8::Initialize();
	v8::Isolate* isolate = v8::Isolate::New();
	{
		v8::Isolate::Scope isolate_scope(isolate);
		// Create a stack-allocated handle scope.
		v8::HandleScope handle_scope;

		context = Quartz::AppContext::New();

		// Loading code.
		context->AddDirScript(data_dir);
		context->AddDirScript(src_dir);
	}
	isolate->Dispose();
	v8::V8::Dispose();
	return 0;
}
