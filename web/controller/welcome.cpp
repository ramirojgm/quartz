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


#include "../web.h"

Web::Controller::Welcome::Welcome()
{
  map("default",sigc::mem_fun(*this,&Welcome::on_default));
}

Web::Controller::Welcome::~Welcome()
{

}

Quartz::RefPtr<Quartz::Result>
Web::Controller::Welcome::on_default(
    const Quartz::RefPtr<Quartz::Context> & context)
{
  if(context->session.get_int("logged"))
    {
      return content("<h1>Session</h1>","text/html");
    }
  else
    {
      return content("<h1>No session</h1>","text/html");
    }
}
