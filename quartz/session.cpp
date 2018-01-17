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

#include "quartz.h"

Quartz::Session::Session()
{

}

Quartz::Session::~Session()
{

}

void
Quartz::Session::set_int(const Glib::ustring & name,gint value)
{
  m_value[name] = std::to_string(value);
}

gint
Quartz::Session::get_int(const Glib::ustring & name)
{
  return std::atoi(m_value[name].c_str());
}

void
Quartz::Session::set_double(const Glib::ustring & name,gdouble value)
{
  m_value[name] = std::to_string(value);
}

gdouble
Quartz::Session::get_double(const Glib::ustring & name)
{
  return std::atof(m_value[name].c_str());
}

void
Quartz::Session::set_string(const Glib::ustring & name,const Glib::ustring & value)
{
  m_value[name] = value;
}

Glib::ustring
Quartz::Session::get_string(const Glib::ustring & name)
{
  return m_value[name];
}

void
Quartz::Session::set_pointer(const Glib::ustring & name,gpointer value)
{
  m_pointer[name] = value;
}

gpointer
Quartz::Session::get_pointer(const Glib::ustring & name)
{
  return m_pointer[name];
}
