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

#ifndef NEXTWEB_UTILS_TYPE_LIST_HPP_INCLUDED
#define NEXTWEB_UTILS_TYPE_LIST_HPP_INCLUDED

#include <cstddef>
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace utils {

template <typename T, typename Tail>
struct TypeList {
	typedef T Value;
	typedef Tail Next;
	typedef TypeList<T, Tail> Type;
};

struct NullType {
};

template <typename List, typename X>
struct TypeListReverseImpl;

template <typename List>
struct TypeListReverse {
	typedef typename TypeListReverseImpl<List, NullType>::Type Type;
};

template <typename List, typename X>
struct TypeListReverseImpl {
	typedef typename TypeListReverseImpl<typename List::Next, typename TypeList<typename List::Value, X>::Type>::Type Type;
};

template <typename X>
struct TypeListReverseImpl<NullType, X> {
	typedef X Type;
};

template <typename List, std::size_t N>
struct TypeListNthItem {
	typedef typename TypeListNthItem<typename List::Next, N - 1>::Type Type;
};

template <typename List>
struct TypeListNthItem<List, static_cast<std::size_t>(0)> {
	typedef typename List::Value Type;
};

template <std::size_t N>
struct TypeListNthItem<NullType, N> {
	typedef NullType Type;
};

template <typename X, typename List, int Index> 
struct TypeListIndexOfImpl;

template <typename List, typename X>
struct TypeListIndexOf {
	static int const RESULT = TypeListIndexOfImpl<List, X, 0>::RESULT;
};

template <typename List, typename X> int const
TypeListIndexOf<List, X>::RESULT;

template <typename List, typename X, int Index> 
struct TypeListIndexOfImpl {
	static int const RESULT = IsSame<X, typename List::Value>::RESULT ? Index : TypeListIndexOfImpl<typename List::Next, X, Index + 1>::RESULT;
};

template <typename List, typename X, int Index> int const
TypeListIndexOfImpl<List, X, Index>::RESULT;

template <typename X, int Index>
struct TypeListIndexOfImpl<NullType, X, Index> {
	static int const RESULT = -1;
};

template <typename X, int Index> int const
TypeListIndexOfImpl<NullType, X, Index>::RESULT;

}} // namespaces

#define NEXTWEB_MAKE_TYPE_LIST1(T1) utils::TypeList<T1, utils::NullType>
#define NEXTWEB_MAKE_TYPE_LIST2(T1, T2) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST1(T2) >
#define NEXTWEB_MAKE_TYPE_LIST3(T1, T2, T3) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST2(T2, T3) >
#define NEXTWEB_MAKE_TYPE_LIST4(T1, T2, T3, T4) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST3(T2, T3, T4) >
#define NEXTWEB_MAKE_TYPE_LIST5(T1, T2, T3, T4, T5) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST4(T2, T3, T4, T5) >
#define NEXTWEB_MAKE_TYPE_LIST6(T1, T2, T3, T4, T5, T6) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST5(T2, T3, T4, T5, T6) >
#define NEXTWEB_MAKE_TYPE_LIST7(T1, T2, T3, T4, T5, T6, T7) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST6(T2, T3, T4, T5, T6, T7) >
#define NEXTWEB_MAKE_TYPE_LIST8(T1, T2, T3, T4, T5, T6, T7, T8) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST7(T2, T3, T4, T5, T6, T7, T8) >
#define NEXTWEB_MAKE_TYPE_LIST9(T1, T2, T3, T4, T5, T6, T7, T8, T9) utils::TypeList<T1, NEXTWEB_MAKE_TYPE_LIST8(T2, T3, T4, T5, T6, T7, T8, T9) >

#endif // NEXTWEB_UTILS_TYPE_LIST_HPP_INCLUDED
