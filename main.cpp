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

#include <iostream>
#include "quartz/quartz.h"

bool
service_run(
    const Glib::RefPtr<Gio::SocketConnection> & socket_connection,
    const Glib::RefPtr<Glib::Object> & source)
{
  Glib::RefPtr<Gio::InputStream> in = socket_connection->get_input_stream();
  Glib::RefPtr<Gio::DataOutputStream> out = Gio::DataOutputStream::create(socket_connection->get_output_stream());

  Quartz::RefPtr<Quartz::Web::HttpRequest>
  request = Quartz::Web::HttpRequest::create(in);

  std::cout<<"Request:"<<request->get_query()<<std::endl;

  Quartz::RefPtr<Quartz::Web::HttpResponse>
  response = Quartz::Web::HttpResponse::create();
  response->set("Server","Quartz/1.0.0");
  response->set("Content-Length","10");
  response->set("Content-Type","text/html; charset=iso-8859-1");

  response->write(out);
  out->write("Hello Word");
  return true;
}

gint
main(gint argc,gchar ** argv)
{
  Gio::init();
  Glib::RefPtr<Gio::ThreadedSocketService> service = Gio::ThreadedSocketService::create(128);
  Glib::RefPtr<Glib::MainLoop> main = Glib::MainLoop::create();

  service->add_inet_port(8080);
  service->signal_run().connect(sigc::ptr_fun(&service_run));
  service->start();

  main->run();
  return 0;
}
