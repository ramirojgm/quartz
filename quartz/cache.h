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

#ifndef QUARTZ_CACHE_H_
#define QUARTZ_CACHE_H_

namespace Quartz
{
  class Cache
  {
  public:
    Cache(const Glib::ustring & filename);

    virtual ~Cache();

    const guint8 * get_data(
	Web::HttpCompressionType type,
	Glib::ustring & mime_type,
	Glib::ustring & modification_time,
	gsize & size);


  private:
    Glib::ustring m_mimetype;
    Glib::ustring m_modification_time;

    guint8 *	m_uncompress_data;
    gsize	m_uncompress_size;
    guint8 *	m_gzip_data;
    gsize	m_gzip_size;
    guint8 *	m_deflate_data;
    gsize 	m_deflate_size;
  };
}

#endif /* QUARTZ_CACHE_H_ */
