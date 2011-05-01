// nextweb - modern web framework for Python and C++
// Copyright (C) 2011 Oleg Obolenskiy <highpower@yandex-team.ru>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef NEXTWEB_ENUMERATION_HPP_INCLUDED
#define NEXTWEB_ENUMERATION_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/Shared.hpp"
#include "nextweb/SharedPtr.hpp"

namespace nextweb {

template <typename Item>
class NEXTWEB_API Enumeration : public Shared {

public:
	Enumeration();
	virtual ~Enumeration();
	
	typedef Item ValueType;
	typedef Enumeration<Item> Type;
	typedef SharedPtr<Type> Pointer;

	virtual Item nextElement() const = 0;
	virtual bool hasMoreElements() const = 0;

private:
	Enumeration(Enumeration const &);
	Enumeration& operator = (Enumeration const &);
};

template <typename Item> NEXTWEB_INLINE
Enumeration<Item>::Enumeration()
{
}

template <typename Item> NEXTWEB_INLINE
Enumeration<Item>::~Enumeration() {
}

} // namespace 

#endif // NEXTWEB_ENUMERATION_HPP_INCLUDED
