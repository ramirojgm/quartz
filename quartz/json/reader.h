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

#ifndef QUARTZ_JSON_READER_H_
#define QUARTZ_JSON_READER_H_

#include "json-glib/json-glib.h"

namespace Quartz
{
  namespace JSON
  {
    class Reader {
    public:
      Reader(const Glib::ustring & src);

      virtual ~Reader();

      bool is_object();
      bool is_array();
      bool is_value();
      bool is_null();

      gint count_elements();
      gint count_members();

      Glib::ustring get_member_name();

      bool read_element(gint index);
      void end_element();

      bool read_member(const Glib::ustring & name);
      void end_member();

      gint64 read_int();
      gdouble read_double();
      Glib::ustring read_string();
      bool read_boolean();

    private:
      JsonReader * m_reader;
    };
  }
}


#endif /* QUARTZ_DATA_MODEL_H_ */
