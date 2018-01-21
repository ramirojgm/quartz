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

#include "../quartz.h"

Quartz::JSON::Reader::Reader(const Glib::ustring & source)
{
  JsonParser * parser = json_parser_new();
  json_parser_load_from_data(parser,source.c_str(),source.length(),NULL);
  m_reader = json_reader_new(json_node_ref(json_parser_get_root(parser)));
  g_object_unref(G_OBJECT(parser));
}

Quartz::JSON::Reader::~Reader()
{
  g_object_unref(G_OBJECT(m_reader));
}

bool
Quartz::JSON::Reader::is_object()
{
  return json_reader_is_object(m_reader);
}

bool
Quartz::JSON::Reader::is_array()
{
  return json_reader_is_array(m_reader);
}

bool
Quartz::JSON::Reader::is_value()
{
  return json_reader_is_value(m_reader);
}

bool
Quartz::JSON::Reader::is_null()
{
  return json_reader_get_null_value(m_reader);
}

gint
Quartz::JSON::Reader::count_elements()
{
  return json_reader_count_elements(m_reader);
}


gint
Quartz::JSON::Reader::count_members()
{
  return json_reader_count_members(m_reader);
}

Glib::ustring
Quartz::JSON::Reader::get_member_name()
{
  return Glib::ustring(json_reader_get_member_name(m_reader));
}

bool
Quartz::JSON::Reader::read_element(gint index)
{
  return json_reader_read_element(m_reader,index);
}

void
Quartz::JSON::Reader::end_element()
{
  json_reader_end_element(m_reader);
}

bool
Quartz::JSON::Reader::read_member(const Glib::ustring & name)
{
  return json_reader_read_member(m_reader,name.c_str());
}

void
Quartz::JSON::Reader::end_member()
{
  json_reader_end_member(m_reader);
}

gint64
Quartz::JSON::Reader::read_int()
{
  return json_reader_get_int_value(m_reader);
}

gdouble
Quartz::JSON::Reader::read_double()
{
  return json_reader_get_double_value(m_reader);
}

Glib::ustring
Quartz::JSON::Reader::read_string()
{
  return Glib::ustring(json_reader_get_string_value(m_reader));
}

bool
Quartz::JSON::Reader::read_boolean()
{
  return json_reader_get_boolean_value(m_reader);
}
