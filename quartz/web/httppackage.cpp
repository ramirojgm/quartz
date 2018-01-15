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


#include <string>
#include <string.h>

#include "../quartz.h"

Quartz::Web::HttpPackage::HttpPackage()
{

}

Quartz::Web::HttpPackage::~HttpPackage()
{

}

Glib::RefPtr<Gio::DataInputStream>
Quartz::Web::HttpPackage::buffer_stream(
    const Glib::RefPtr<Gio::InputStream> & stream)
{
  Glib::RefPtr<Gio::MemoryOutputStream> out = Gio::MemoryOutputStream::create(NULL,0,g_realloc,g_free);
  Glib::RefPtr<Gio::MemoryInputStream> input;
  Glib::RefPtr<Gio::DataInputStream> buffer;

  gchar * content = NULL;
  guint8 byte = 0;
  gsize nbyte;
  while(stream->read(&byte,1))
    {
      if(out->get_data_size() > 2048)
	{
	  out->close();
	  g_free(out->steal_data());
	  return buffer;
	}

      out->write_all(&byte,1,nbyte);
      if(out->get_data_size() >= 4)
	{
	  content = (gchar*)out->get_data();
	  if(strncmp((content + out->get_data_size() - 4),"\r\n\r\n",4) == 0)
	  break;
	}
    }
  out->close();

  input = Gio::MemoryInputStream::create();
  input->add_data(out->steal_data(),out->get_data_size(),g_free);

  buffer = Gio::DataInputStream::create(input);
  buffer->set_newline_type(Gio::DATA_STREAM_NEWLINE_TYPE_CR_LF);
  return buffer;

}

void
Quartz::Web::HttpPackage::read_attributes(const Glib::RefPtr<Gio::DataInputStream> & stream)
{
  m_attributes.clear();
  std::string line;
  gsize line_length;
  std::string::size_type sep_pos = 0;

  for(;;)
    {
      stream->read_line_utf8(line,line_length);
      if(line_length == 0)
	break;

      if((sep_pos = line.find(":")) != std::string::npos)
	{
	  std::string name(line.substr(0,sep_pos));
	  std::string data(line.substr(sep_pos + 1));
	  m_attributes[name] = data;
	}
      else
	{
	  break;
	}
    }
}

void
Quartz::Web::HttpPackage::write_attributes(const Glib::RefPtr<Gio::DataOutputStream> & stream)
{
  gsize written = 0;
  std::map<Glib::ustring,Glib::ustring>::iterator it;
  Glib::ustring line = "";
  for(it = m_attributes.begin();it != m_attributes.end(); it++)
      stream->write_all(Glib::ustring::compose("%1:%2\r\n",it->first,it->second),written);
  stream->write("\r\n");
}


bool
Quartz::Web::HttpPackage::has(
    const Glib::ustring & name)
{
  return m_attributes.find(name) != m_attributes.end();
}

void
Quartz::Web::HttpPackage::reset()
{
  m_attributes.clear();
}

gint
Quartz::Web::HttpPackage::get_int(
  const Glib::ustring & name)
{
  return std::atoi(m_attributes[name].c_str());
}

gdouble
Quartz::Web::HttpPackage::get_double(
  const Glib::ustring & name)
{
  return std::atof(m_attributes[name].c_str());
}

gint64
Quartz::Web::HttpPackage::get_int64(
   const Glib::ustring & name)
{
   return std::atoll(m_attributes[name].c_str());
}

Glib::ustring
Quartz::Web::HttpPackage::get_string(
    const Glib::ustring & name)
{
  return m_attributes[name];
}

void
Quartz::Web::HttpPackage::set(
    const Glib::ustring & name,
    gint64 value)
{
  m_attributes[name] = std::to_string(value);
}

void
Quartz::Web::HttpPackage::set(
    const Glib::ustring & name,
    gdouble value)
{
  m_attributes[name] = std::to_string(value);
}

void
Quartz::Web::HttpPackage::set(
    const Glib::ustring & name,
    const Glib::ustring & value)
{
  m_attributes[name] = value;
}
