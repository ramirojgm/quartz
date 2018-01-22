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

#ifndef QUARTZ_CONTROLLER_H_
#define QUARTZ_CONTROLLER_H_

namespace Quartz
{
  typedef sigc::slot1<RefPtr<Result>,const RefPtr<Context> &> ActionFunc;

  class Controller
  {
  public:
    Controller();
    virtual ~Controller();

    bool has(const Glib::ustring & name);

    RefPtr<Result> invoke(const Glib::ustring & name,const RefPtr<Context> & context);


    RefPtr<Result> content(const Glib::ustring & content,
			   const Glib::ustring & content_type);

    RefPtr<Result> redirect(const Glib::ustring & url);

  protected:
    void map(
	const Glib::ustring & name,
	const ActionFunc & func);

  private:
    std::map<Glib::ustring,ActionFunc> m_action;
  };
}

#endif /* QUARTZ_CONTROLLER_H_ */
