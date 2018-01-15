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

#ifndef QUARTZ_WEB_HTTPPACKAGE_H_
#define QUARTZ_WEB_HTTPPACKAGE_H_

namespace Quartz
{
  namespace Web
  {
    class HttpPackage
    {
    protected:
      HttpPackage();

      void read_attributes(const Glib::RefPtr<Gio::DataInputStream> & stream);

      void write_attributes(const Glib::RefPtr<Gio::DataOutputStream> & stream);

    public:
      virtual ~HttpPackage();

      static Glib::RefPtr<Gio::DataInputStream> buffer_stream(const Glib::RefPtr<Gio::InputStream> & stream);

      bool has(const Glib::ustring & name);

      void reset();

      gint	 get_int(const Glib::ustring & name);
      gint64	 get_int64(const Glib::ustring & name);
      gdouble 	 get_double(const Glib::ustring & name);
      Glib::ustring get_string(const Glib::ustring & name);

      void set(const Glib::ustring & name,gint64 value);
      void set(const Glib::ustring & name,gdouble value);
      void set(const Glib::ustring & name,const Glib::ustring & value);

    private:
      std::map<Glib::ustring,Glib::ustring> m_attributes;
    };
  }
}

#endif /* QUARTZ_WEB_HTTPPACKAGE_H_ */
