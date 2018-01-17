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

#ifndef QUARTZ_SESSION_H_
#define QUARTZ_SESSION_H_

namespace Quartz
{
  class Session
  {
  private:
    friend class Application;

    Session();

  public:

    virtual ~Session();

    void set_int(const Glib::ustring & name,gint value);
    gint get_int(const Glib::ustring & name);

    void set_double(const Glib::ustring & name,gdouble value);
    gdouble get_double(const Glib::ustring & name);

    void set_string(const Glib::ustring & name,const Glib::ustring & value);
    Glib::ustring get_string(const Glib::ustring & name);

    void set_pointer(const Glib::ustring & name,gpointer value);
    gpointer get_pointer(const Glib::ustring & name);

  private:
    std::map<Glib::ustring,Glib::ustring> m_value;
    std::map<Glib::ustring,gpointer> m_pointer;
  };
}

#endif /* QUARTZ_SESSION_H_ */
