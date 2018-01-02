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

#ifndef HTTP_PACKAGE_
#define HTTP_PACKAGE_

namespace Quartz
{
	class HttpPackage
	{
	protected:
		HttpPackage();

	public:
		virtual ~HttpPackage();

		static Glib::RefPtr<Gio::InputStream> BufferStream(const Glib::RefPtr<Gio::InputStream> & stream);

		virtual void ReadFromStream(const Glib::RefPtr<Gio::InputStream> & stream,const Glib::RefPtr<Glib::Cancellable> & cancellable);
		virtual void WriteToStream(const Glib::RefPtr<Gio::OutputStream> & stream,const Glib::RefPtr<Glib::Cancellable> & cancellable);

		bool Has(const std::string & name);
		void Unset(const std::string & name);
		void Reset();

		void Set(const std::string & name,int value);
		void Set(const std::string & name,double value);
		void Set(const std::string & name,const std::string & value);

		int			GetInt32(const std::string & name);
		double		GetDouble(const std::string & name);
		std::string	GetString(const std::string & name);

	private:
		std::map<std::string, std::string> m_attributes;
	};
}

#endif
