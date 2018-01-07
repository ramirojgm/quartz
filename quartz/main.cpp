#include "quartz.h"

static std::string site_dir;
static std::string data_dir;
static std::string src_dir;
static std::string content_dir;

static Quartz::RefPtr<Quartz::AppContext> context;

static Glib::RefPtr<Glib::MainLoop> 	loop;
static Glib::RefPtr<Gio::ThreadedSocketService> service;

bool Quartz_Service_Run(
	const Glib::RefPtr<Gio::SocketConnection> & connection,
	const Glib::RefPtr<Glib::Object> & source);

int main(int argc,char ** argv)
{
	site_dir = Glib::get_current_dir();
	if(argc > 1)
		site_dir = argv[1];

	data_dir = Glib::build_filename(site_dir,"js/data/");
	src_dir = Glib::build_filename(site_dir,"js/src/");
	content_dir = Glib::build_filename(site_dir,"content/");

	loop = Glib::MainLoop::create();
	service = Gio::ThreadedSocketService::create(200);
	//service->add_inet_port(8080);

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

/*		service->signal_run().connect(sigc::ptr_fun(&Quartz_Service_Run));
		service->start();*/
		loop->run();
	}
	isolate->Dispose();
	v8::V8::Dispose();
	return 0;
}

bool Quartz_Service_Run(
	const Glib::RefPtr<Gio::SocketConnection> & connection,
	const Glib::RefPtr<Glib::Object> & source)
{
	std::cout<<"OK"<<std::endl;
	return true;
}