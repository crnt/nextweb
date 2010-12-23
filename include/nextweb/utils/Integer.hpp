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

#ifndef NEXTWEB_UTILS_INTEGER_HPP_INCLUDED
#define NEXTWEB_UTILS_INTEGER_HPP_INCLUDED

#include <limits>
#include <cstddef>

#include "nextweb/utils/TypeList.hpp"
#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/StaticAssert.hpp"

namespace nextweb { namespace utils {

typedef NEXTWEB_MAKE_TYPE_LIST5(signed char, signed short, signed int, signed long, signed long long) SignedIntegerList;
typedef NEXTWEB_MAKE_TYPE_LIST5(unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long) UnsignedIntegerList;

template <typename X>
struct IsInt {
	static bool const RESULT = (TypeListIndexOf<SignedIntegerList, X>::RESULT != -1) || (TypeListIndexOf<UnsignedIntegerList, X>::RESULT != -1);
};

template <>
struct IsInt<char> {
	static bool const RESULT = true;
};

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

typedef IntegerSizeSearch<1, SignedIntegerList>::Type Int8;
typedef IntegerSizeSearch<2, SignedIntegerList>::Type Int16;
typedef IntegerSizeSearch<4, SignedIntegerList>::Type Int32;
typedef IntegerSizeSearch<8, SignedIntegerList>::Type Int64;
typedef Int64 IntMax;

typedef IntegerSizeSearch<1, UnsignedIntegerList>::Type UInt8;
typedef IntegerSizeSearch<2, UnsignedIntegerList>::Type UInt16;
typedef IntegerSizeSearch<4, UnsignedIntegerList>::Type UInt32;
typedef IntegerSizeSearch<8, UnsignedIntegerList>::Type UInt64;
typedef UInt64 UIntMax;

template <bool IsSigned>
struct MaxIntImpl;

template <typename X>
struct MaxInt {
	typedef typename MaxIntImpl<std::numeric_limits<X>::is_signed>::Type Type;
};

template <>
struct MaxIntImpl<true> {
	typedef IntMax Type;
};

template <>
struct MaxIntImpl<false> {
	typedef UIntMax Type;
};

template <typename X>
struct MakeSigned {
	NEXTWEB_STATIC_ASSERT(IsInt<X>::RESULT);
	static int const SP = TypeListIndexOf<SignedIntegerList, X>::RESULT;
	static int const UP = TypeListIndexOf<UnsignedIntegerList, X>::RESULT;
	typedef typename TypeListNthItem<SignedIntegerList, (-1 != SP) ? SP : UP>::Type Type;
};

template <typename X> int const
MakeSigned<X>::SP;

template <typename X> int const
MakeSigned<X>::UP;

template <>
struct MakeSigned<char> {
	typedef signed char Type;
};

template <typename X>
struct MakeUnsigned {
	NEXTWEB_STATIC_ASSERT(IsInt<X>::RESULT);
	static int const SP = TypeListIndexOf<SignedIntegerList, X>::RESULT;
	static int const UP = TypeListIndexOf<UnsignedIntegerList, X>::RESULT;
	typedef typename TypeListNthItem<UnsignedIntegerList, (-1 != SP) ? SP : UP>::Type Type;
};


template <typename X> int const
MakeUnsigned<X>::SP;

template <typename X> int const
MakeUnsigned<X>::UP;

template <>
struct MakeUnsigned<char> {
	typedef unsigned char Type;
};

}} // namespaces

#endif // NEXTWEB_UTILS_INTEGER_HPP_INCLUDED
