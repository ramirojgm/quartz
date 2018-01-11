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

#ifndef QUARTZ_DATA_ARRAY_H_
#define QUARTZ_DATA_ARRAY_H_

namespace Quartz
{
  namespace Data
  {
    class Array: public SerializableBase {
    public:
      typedef std::vector<Quartz::RefPtr<Quartz::Data::SerializableBase> >::iterator iterator;

    public:
      Array();

      virtual ~Array();

      iterator begin();

      iterator end();

      template<typename serializable_t> void push(const serializable_t & t);

      void push(const Quartz::RefPtr<Quartz::Data::SerializableBase> & value);

      void clear();

      void erase(const iterator & it);

      void parse(const Glib::RefPtr<Gio::DataInputStream> & input,
		  const Glib::RefPtr<Gio::Cancellable> & cancellable);

      void serialize(
	  const Glib::RefPtr<Gio::DataOutputStream> & output,
	  const Glib::RefPtr<Gio::Cancellable> & cancellable);

    private:
      std::vector<Quartz::RefPtr<Quartz::Data::SerializableBase> > m_items;
    };
  }
}


#endif /* QUARTZ_DATA_ARRAY_H_ */
