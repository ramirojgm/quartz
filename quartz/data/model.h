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

#ifndef QUARTZ_DATA_MODEL_H_
#define QUARTZ_DATA_MODEL_H_

namespace Quartz
{
  namespace Data
  {
    class Model: public SerializableBase {
    public:
      typedef std::map<Glib::ustring,Quartz::RefPtr<Quartz::Data::SerializableBase> >::iterator iterator;

    protected:
      Model();

      void map(const Glib::ustring & name,Quartz::Data::SerializableBase & attr);

      void parse(const Glib::RefPtr<Gio::DataInputStream> & input,
      		  const Glib::RefPtr<Gio::Cancellable> & cancellable);

      void serialize(
      	  const Glib::RefPtr<Gio::DataOutputStream> & output,
      	  const Glib::RefPtr<Gio::Cancellable> & cancellable);

    public:
      virtual ~Model();

      iterator begin();

      iterator end();

    private:
      std::map<Glib::ustring,Quartz::RefPtr<Quartz::Data::SerializableBase> > m_attributes;
    };
  }
}


#endif /* QUARTZ_DATA_MODEL_H_ */
