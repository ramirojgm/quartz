/*
	Copyright (C) 2017 Ramiro Jose Garcia Moraga

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
 #include <string.h>
 #include <string>

Quartz::HttpPackage::HttpPackage()
{


}

Quartz::HttpPackage::~HttpPackage()
{


}

Glib::RefPtr<Gio::InputStream> 
Quartz::HttpPackage::BufferStream(const Glib::RefPtr<Gio::InputStream> & stream)
{
	Glib::RefPtr<Gio::MemoryOutputStream> out = Gio::MemoryOutputStream::create();
	Glib::RefPtr<Gio::MemoryInputStream> input;
	gchar * content = NULL;
	guint8 byte = 0;
	gsize nbyte;
	while(stream->read(&byte,1))
	{
		if(out->get_data_size() > 2048)
		{
			out->close();
			g_free(out->steal_data());
			return input;
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
		
	Glib::RefPtr<Gio::DataInputStream> dis = Gio::DataInputStream::create(input);
	dis->set_newline_type(Gio::DATA_STREAM_NEWLINE_TYPE_CR_LF);

	return dis;
}


void 
Quartz::HttpPackage::ReadFromStream(
	const Glib::RefPtr<Gio::DataInputStream> & stream,
	const Glib::RefPtr<Gio::Cancellable> & cancellable)
{
	std::string line = "";
	gsize line_length = 0;
	std::string::size_type sep_pos = 0;

	m_attributes.clear();

	for(;;)
	{
		stream->read_line_utf8(line,cancellable,line_length);
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
Quartz::HttpPackage::WriteToStream(
	const Glib::RefPtr<Gio::OutputStream> & stream,
	const Glib::RefPtr<Gio::Cancellable> & cancellable)
{
	gsize written = 0;
	for(std::map<std::string,std::string>::iterator it = m_attributes.begin(); it != m_attributes.end(); it ++)
	{
		std::pair<std::string,std::string> attr = *it;
		stream->write_all(attr.first,written,cancellable);
		stream->write_all(":",written,cancellable);
		stream->write_all(attr.second,written,cancellable);
		stream->write_all("\r\n",written,cancellable);
	}
	stream->write_all("\r\n",written,cancellable);
}

bool 
Quartz::HttpPackage::Has(
	const std::string & name)
{
	return m_attributes.find(name) != m_attributes.end();
}

void 
Quartz::HttpPackage::Unset(
	const std::string & name)
{
	std::map<std::string,std::string>::iterator it = m_attributes.find(name);
	if(it != m_attributes.end())
	{
		m_attributes.erase(it);
	}
}

void 
Quartz::HttpPackage::Reset()
{
	m_attributes.clear();
}

void 
Quartz::HttpPackage::Set(
	const std::string & name,
	int value)
{
	m_attributes[name] = std::to_string(value);
}

void 
Quartz::HttpPackage::Set(
	const std::string & name,
	double value)
{
	m_attributes[name] = std::to_string(value);
}

void 
Quartz::HttpPackage::Set(
	const std::string & name,
	const std::string & value)
{
	m_attributes[name] = value;
}

int
Quartz::HttpPackage::GetInt32(const std::string & name)
{
	if(Has(name))
		return stoi(m_attributes[name]);
	else
		return 0;
}

double		
Quartz::HttpPackage::GetDouble(const std::string & name)
{
	if(Has(name))
		return stod(m_attributes[name]);
	else
		return 0.0;
}

std::string	
Quartz::HttpPackage::GetString(const std::string & name)
{
	if(Has(name))
		return m_attributes[name];
	else
		return "";
}
