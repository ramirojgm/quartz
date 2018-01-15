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

Quartz::Cache::Cache(const Glib::ustring & filename)
{
  Glib::ustring file_data = Glib::file_get_contents(filename);
  Glib::DateTime cur_time = Glib::DateTime::create_now_local();

  gsize written = 0;

  m_mimetype = "text/html";
  m_modification_time = cur_time.format("%a, %e %b %Y %T %Z");

  if(file_data.length() > 0)
    {
      m_uncompress_data = (guint8*)g_memdup(file_data.c_str(),file_data.length());
      m_uncompress_size = file_data.length();

      Glib::RefPtr<Gio::ZlibCompressor> gzip_compressor = Gio::ZlibCompressor::create(Gio::ZLIB_COMPRESSOR_FORMAT_GZIP,9);
      Glib::RefPtr<Gio::ZlibCompressor> deflate_compressor = Gio::ZlibCompressor::create(Gio::ZLIB_COMPRESSOR_FORMAT_RAW,9);
      Glib::RefPtr<Gio::MemoryOutputStream> gzip_out = Gio::MemoryOutputStream::create();
      Glib::RefPtr<Gio::MemoryOutputStream> deflate_out = Gio::MemoryOutputStream::create();
      Glib::RefPtr<Gio::ConverterOutputStream> gzip_converter = Gio::ConverterOutputStream::create(gzip_out,gzip_compressor);
      Glib::RefPtr<Gio::ConverterOutputStream> deflate_converter = Gio::ConverterOutputStream::create(deflate_out,deflate_compressor);

      gzip_converter->write_all((gchar*)m_uncompress_data,m_uncompress_size,written);
      deflate_converter->write_all((gchar*)m_uncompress_data,m_uncompress_size,written);

      gzip_converter->flush();
      deflate_converter->flush();

      gzip_converter->close();
      deflate_converter->close();

      m_gzip_size = gzip_out->get_data_size();
      m_gzip_data = (guint8*)gzip_out->steal_data();

      m_deflate_size = deflate_out->get_data_size();
      m_deflate_data = (guint8*)deflate_out->steal_data();
    }
  else
    {
      m_uncompress_size = 0;
      m_uncompress_data = NULL;

      m_gzip_size = 0;
      m_gzip_data = NULL;

      m_deflate_size = 0;
      m_deflate_data = NULL;
    }
}

Quartz::Cache::~Cache()
{
  g_free(m_uncompress_data);
  g_free(m_gzip_data);
  g_free(m_deflate_data);
}

const guint8 *
Quartz::Cache::get_data(
    Quartz::Web::HttpCompressionType type,
    Glib::ustring & mime_type,
    Glib::ustring & modification_time,
    gsize & size)
{
  mime_type = m_mimetype;
  modification_time = m_modification_time;

  switch(type)
  {
    case Web::HTTP_COMPRESSION_GZIP:
      size = m_gzip_size;
      return m_gzip_data;
    case Web::HTTP_COMPRESSION_DEFLATE:
      size = m_deflate_size;
      return m_deflate_data;
    default:
      size = m_uncompress_size;
      return m_uncompress_data;
  }
  return NULL;
}
