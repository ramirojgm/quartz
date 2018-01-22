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


Quartz::Controller::Controller()
{

}

Quartz::Controller::~Controller()
{

}

bool
Quartz::Controller::has(const Glib::ustring & name)
{
  return m_action.find(name) != m_action.end();
}

Quartz::RefPtr<Quartz::Result>
Quartz::Controller::invoke(
    const Glib::ustring & name,
    const Quartz::RefPtr<Quartz::Context> & context)
{
  Quartz::ActionFunc func = m_action[name];
  return func(context);
}

Quartz::RefPtr<Quartz::Result>
Quartz::Controller::content(const Glib::ustring & content,
			    const Glib::ustring & content_type)
{
  return Quartz::RefPtr<Result>(new ContentResult(content,content_type));
}

void
Quartz::Controller::map(
    const Glib::ustring & name,
    const Quartz::ActionFunc & func)
{
  m_action[name] = func;
}
