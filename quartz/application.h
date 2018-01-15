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

#ifndef QUARTZ_APPLICATION_H_
#define QUARTZ_APPLICATION_H_

namespace Quartz
{
  class Application
  {
  public:
    Application();

    virtual ~Application();

    void add_inet_port(guint16 inet_port);

    void map(const Glib::ustring & vpath,const Glib::ustring & path);

    template<typename T_Controller>
    void map(const Glib::ustring & vpath);

    void run();

    void quit();

  private:
    bool on_service_run(
	const Glib::RefPtr<Gio::SocketConnection> &  connection,
	const Glib::RefPtr<Glib::Object> & source);

    Glib::RefPtr<Gio::ThreadedSocketService> m_service;
    Glib::RefPtr<Glib::MainLoop> m_main_loop;

    GMutex m_controller_mutex;
    std::map<Glib::ustring,Controller*> m_controller;

    GMutex m_cache_mutex;
    std::map<Glib::ustring,Cache*> m_cache;
  };
}

#endif /* QUARTZ_APPLICATION_H_ */
