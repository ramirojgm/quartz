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

#ifndef QUARTZ_DATA_SERIALIZABLE_H_
#define QUARTZ_DATA_SERIALIZABLE_H_

namespace Quartz
{
  namespace Data
  {
    template<typename serializable_t>
    class Serializable: public SerializableBase {

    public:
      Serializable();

      virtual ~Serializable();

      virtual void parse(const Glib::RefPtr<Gio::DataInputStream> & input,
      		  const Glib::RefPtr<Gio::Cancellable> & cancellable);

      virtual void serialize(
      	  const Glib::RefPtr<Gio::DataOutputStream> & output,
      	  const Glib::RefPtr<Gio::Cancellable> & cancellable);


      Serializable<serializable_t> & operator=(const serializable_t & new_value)
      {
	m_value = new_value;
	return *this;
      }

      Serializable<serializable_t> & operator=(const Serializable<serializable_t> & new_value)
      {
	m_value = new_value.m_value;
	return *this;
      }

      serializable_t & operator()(void)
      {
	return m_value;
      }

    private:
      serializable_t m_value;
    };
  }
}


#endif /* QUARTZ_DATA_SERIALIZABLE_H_ */
