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

#ifndef NEXTWEB_UTILS_PASSING_TYPE_HPP_INCLUDED
#define NEXTWEB_UTILS_PASSING_TYPE_HPP_INCLUDED

#include "nextweb/SharedPtr.hpp"
#include "nextweb/utils/Integer.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace utils {

template <typename T, bool ByVal>
struct PassingTypeImpl;

template <typename T>
struct PassingType {
	typedef typename PassingTypeImpl<T, IsInt<T>::RESULT || IsPointer<T>::RESULT>::Type Type;
};

template <typename T>
struct PassingTypeImpl<T, true> {
	typedef T Type;
};

template <typename T>
struct PassingTypeImpl<T, false> {
	typedef T const& Type;
};

template <typename T>
struct PassingTypeImpl<SharedPtr<T>, false> {
	typedef SharedPtr<T> Type;
};

}} // namespaces

#endif // NEXTWEB_UTILS_PASSING_TYPE_HPP_INCLUDED
