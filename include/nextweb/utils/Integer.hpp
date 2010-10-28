// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef NEXTWEB_UTILS_INTEGER_HPP_INCLUDED
#define NEXTWEB_UTILS_INTEGER_HPP_INCLUDED

#include <limits>
#include <cstddef>

#include "nextweb/utils/TypeList.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace utils {

template <std::size_t Size, typename List, bool IsFound>
struct IntegerSizeSearchImpl;

template <std::size_t Size, typename List>
struct IntegerSizeSearch {
	typedef typename IntegerSizeSearchImpl<Size, List, Size == sizeof(typename List::Value)>::Type Type;
};

template <std::size_t Size, typename List>
struct IntegerSizeSearchImpl<Size, List, true> {
	typedef typename List::Value Type;
};

template <std::size_t Size, typename List>
struct IntegerSizeSearchImpl<Size, List, false> {
	typedef typename IntegerSizeSearch<Size, typename List::Next>::Type Type;
};

template <std::size_t Size, bool IsFound>
struct IntegerSizeSearchImpl<Size, NullType, IsFound> {
	typedef NullType Type;
};

template <typename X, typename List, typename TargerList, bool IsFound>
struct IntegerTypeSearchImpl;

template <typename X, typename List, typename TargetList>
struct IntegerTypeSearch {
	typedef typename IntegerTypeSearchImpl<X, List, TargetList, IsSame<X, typename List::Value>::RESULT || IsSame<X, typename TargetList::Value>::RESULT>::Type Type;
};

template <typename List, typename TargetList>
struct IntegerTypeSearch<char, List, TargetList> {
	typedef typename IntegerTypeSearch<signed char, List, TargetList>::Type Type;
};

template <typename X, typename List, typename TargetList>
struct IntegerTypeSearchImpl<X, List, TargetList, true> {
	typedef typename TargetList::Value Type;
};

template <typename X, typename List, typename TargetList>
struct IntegerTypeSearchImpl<X, List, TargetList, false> {
	typedef typename IntegerTypeSearch<X, typename List::Next, typename TargetList::Next>::Type Type;
};

template <typename X, typename List, bool IsFound>
struct IntegerTypeSearchImpl<X, List, NullType, IsFound> {
	typedef NullType Type;
};

template <typename X, typename TargetList, bool IsFound>
struct IntegerTypeSearchImpl<X, NullType, TargetList, IsFound> {
	typedef NullType Type;
};

typedef NEXTWEB_MAKE_TYPE_LIST5(signed char, signed short, signed int, signed long, signed long long) SignedIntegerList;
typedef NEXTWEB_MAKE_TYPE_LIST5(unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long) UnsignedIntegerList;

typedef IntegerSizeSearch<1, SignedIntegerList>::Type Int8;
typedef IntegerSizeSearch<2, SignedIntegerList>::Type Int16;
typedef IntegerSizeSearch<4, SignedIntegerList>::Type Int32;
typedef IntegerSizeSearch<8, SignedIntegerList>::Type Int64;

typedef IntegerSizeSearch<1, UnsignedIntegerList>::Type UInt8;
typedef IntegerSizeSearch<2, UnsignedIntegerList>::Type UInt16;
typedef IntegerSizeSearch<4, UnsignedIntegerList>::Type UInt32;
typedef IntegerSizeSearch<8, UnsignedIntegerList>::Type UInt64;

template <typename Arg>
struct MakeSigned {
	typedef typename IntegerTypeSearch<Arg, UnsignedIntegerList, SignedIntegerList>::Type Type;
};

template <typename Arg>
struct MakeUnsigned {
	typedef typename IntegerTypeSearch<Arg, SignedIntegerList, UnsignedIntegerList>::Type Type;
};

}} // namespaces

#endif // NEXTWEB_UTILS_INTEGER_HPP_INCLUDED
