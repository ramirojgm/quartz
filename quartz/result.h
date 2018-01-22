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

#ifndef QUARTZ_RESULT_H_
#define QUARTZ_RESULT_H_

namespace Quartz
{
  class Result
  {
  protected:
    Result();

  public:
    virtual ~Result();

    virtual void run(
	const Quartz::RefPtr<Quartz::Web::HttpResponse> & response) {};

    virtual Glib::ustring get_content() = 0;
  };

  class JSONResult:public Result
  {
  private:
    friend class Controller;

    JSONResult();

  public:
    virtual ~JSONResult();

    void run(
	const Quartz::RefPtr<Quartz::Web::HttpResponse> & response);

    Glib::ustring get_content();
  };

  class ContentResult:public Result
  {
  private:
    friend class Controller;

    ContentResult(const Glib::ustring & content,
		  const Glib::ustring & content_type);

  public:

    virtual ~ContentResult();

    void run(
	const Quartz::RefPtr<Quartz::Web::HttpResponse> & response);

    Glib::ustring get_content();

  private:
    Glib::ustring m_content;
    Glib::ustring m_content_type;
  };
}

#endif /* QUARTZ_RESULT_H_ */
