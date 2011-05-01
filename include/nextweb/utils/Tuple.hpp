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

#ifndef NEXTWEB_TUPLE_UTILS_HPP_INCLUDED
#define NEXTWEB_TUPLE_UTILS_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/utils/TypeList.hpp"

namespace nextweb { namespace utils {

template <typename T, typename Tail> 
struct Tuple : public TypeList<T, Tail> {
	T value;
	Tail next;
};

template <typename T, std::size_t N>
struct TupleGetItemImpl {
	static typename TypeListNthItem<T, N>::Type const& getItem(T const &t);
	static void setItem(T &t, typename TypeListNthItem<T, N>::Type const& value);
};

template <typename T>
struct TupleGetItemImpl<T, static_cast<std::size_t>(0)> {
	static typename T::Value const& getItem(T const &t);
	static void setItem(T &t, typename T::Value const& value);
};

template <typename T, std::size_t N> NEXTWEB_INLINE typename TypeListNthItem<T, N>::Type const&
TupleGetItemImpl<T, N>::getItem(T const &t) {
	return TupleGetItemImpl<typename T::Next, N - 1>::getItem(t.next);
}

template <typename T, std::size_t N> NEXTWEB_INLINE void
TupleGetItemImpl<T, N>::setItem(T &t, typename TypeListNthItem<T, N>::Type const& value) {
	TupleGetItemImpl<typename T::Next, N - 1>::setItem(t.next, value);
}

template <typename T> NEXTWEB_INLINE typename T::Value const&
TupleGetItemImpl<T, static_cast<std::size_t>(0)>::getItem(T const &t) {
	return t.value;
}

template <typename T> NEXTWEB_INLINE void
TupleGetItemImpl<T, static_cast<std::size_t>(0)>::setItem(T &t, typename T::Value const& value) {
	t.value = value;
}

template <std::size_t N, typename T> NEXTWEB_INLINE typename TypeListNthItem<T, N>::Type const&
tupleGetItem(T const &t) {
	return TupleGetItemImpl<T, N>::getItem(t);
}

template <std::size_t N, typename T> NEXTWEB_INLINE void
tupleSetItem(T &t, typename TypeListNthItem<T, N>::Type const& value) {
	TupleGetItemImpl<T, N>::setItem(t, value);
}

}} // namespaces

#define NEXTWEB_MAKE_TUPLE1(T1) utils::Tuple<T1, utils::NullType>
#define NEXTWEB_MAKE_TUPLE2(T1, T2) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE1(T2) >
#define NEXTWEB_MAKE_TUPLE3(T1, T2, T3) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE2(T2, T3) >
#define NEXTWEB_MAKE_TUPLE4(T1, T2, T3, T4) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE3(T2, T3, T4) >
#define NEXTWEB_MAKE_TUPLE5(T1, T2, T3, T4, T5) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE4(T2, T3, T4, T5) >
#define NEXTWEB_MAKE_TUPLE6(T1, T2, T3, T4, T5, T6) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE5(T2, T3, T4, T5, T6) >
#define NEXTWEB_MAKE_TUPLE7(T1, T2, T3, T4, T5, T6, T7) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE6(T2, T3, T4, T5, T6, T7) >
#define NEXTWEB_MAKE_TUPLE8(T1, T2, T3, T4, T5, T6, T7, T8) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE7(T2, T3, T4, T5, T6, T7, T8) >
#define NEXTWEB_MAKE_TUPLE9(T1, T2, T3, T4, T5, T6, T7, T8, T9) utils::Tuple<T1, NEXTWEB_MAKE_TUPLE8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define NEXTWEB_TUPLE_INIT_ITEMS1_(TUPLE, P1) tupleSetItem<0>((TUPLE), (P1))
#define NEXTWEB_TUPLE_INIT_ITEMS2_(TUPLE, P1, P2) tupleSetItem<1>((TUPLE), (P2)); NEXTWEB_TUPLE_INIT_ITEMS1_(TUPLE, P1)
#define NEXTWEB_TUPLE_INIT_ITEMS3_(TUPLE, P1, P2, P3) tupleSetItem<2>((TUPLE), (P3)); NEXTWEB_TUPLE_INIT_ITEMS2_(TUPLE, P1, P2)
#define NEXTWEB_TUPLE_INIT_ITEMS4_(TUPLE, P1, P2, P3, P4) tupleSetItem<3>((TUPLE), (P4)); NEXTWEB_TUPLE_INIT_ITEMS3_(TUPLE, P1, P2, P3)
#define NEXTWEB_TUPLE_INIT_ITEMS5_(TUPLE, P1, P2, P3, P4, P5) tupleSetItem<4>((TUPLE), (P5)); NEXTWEB_TUPLE_INIT_ITEMS4_(TUPLE, P1, P2, P3, P4)
#define NEXTWEB_TUPLE_INIT_ITEMS6_(TUPLE, P1, P2, P3, P4, P5, P6) tupleSetItem<5>((TUPLE), (P6)); NEXTWEB_TUPLE_INIT_ITEMS5_(TUPLE, P1, P2, P3, P4, P5)
#define NEXTWEB_TUPLE_INIT_ITEMS7_(TUPLE, P1, P2, P3, P4, P5, P6, P7) tupleSetItem<6>((TUPLE), (P7)); NEXTWEB_TUPLE_INIT_ITEMS6_(TUPLE, P1, P2, P3, P4, P5, P6)
#define NEXTWEB_TUPLE_INIT_ITEMS8_(TUPLE, P1, P2, P3, P4, P5, P6, P7, P8) tupleSetItem<7>((TUPLE), (P8)); NEXTWEB_TUPLE_INIT_ITEMS7_(TUPLE, P1, P2, P3, P4, P5, P6, P7)
#define NEXTWEB_TUPLE_INIT_ITEMS9_(TUPLE, P1, P2, P3, P4, P5, P6, P7, P8, P9) tupleSetItem<8>((TUPLE), (P9)); NEXTWEB_TUPLE_INIT_ITEMS8_(TUPLE, P1, P2, P3, P4, P5, P6, P7, P8)

namespace nextweb { namespace utils {
	
template <typename T1> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE1(T1)
makeTuple(T1 const &t1) {
	NEXTWEB_MAKE_TUPLE1(T1) result;
	NEXTWEB_TUPLE_INIT_ITEMS1_(result, t1);
	return result;
}

template <typename T1, typename T2> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE2(T1, T2)
makeTuple(T1 const &t1, T2 const &t2) {
	NEXTWEB_MAKE_TUPLE2(T1, T2) result;
	NEXTWEB_TUPLE_INIT_ITEMS2_(result, t1, t2);
	return result;
}

template <typename T1, typename T2, typename T3> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE3(T1, T2, T3)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3) {
	NEXTWEB_MAKE_TUPLE3(T1, T2, T3) result;
	NEXTWEB_TUPLE_INIT_ITEMS3_(result, t1, t2, t3);
	return result;
}

template <typename T1, typename T2, typename T3, typename T4> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE4(T1, T2, T3, T4)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3, T4 const &t4) {
	NEXTWEB_MAKE_TUPLE4(T1, T2, T3, T4) result;
	NEXTWEB_TUPLE_INIT_ITEMS4_(result, t1, t2, t3, t4);
	return result;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE5(T1, T2, T3, T4, T5)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3, T4 const &t4, T5 const &t5) {
	NEXTWEB_MAKE_TUPLE5(T1, T2, T3, T4, T5) result;
	NEXTWEB_TUPLE_INIT_ITEMS5_(result, t1, t2, t3, t4, t5);
	return result;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE6(T1, T2, T3, T4, T5, T6)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3, T4 const &t4, T5 const &t5, T6 const &t6) {
	NEXTWEB_MAKE_TUPLE6(T1, T2, T3, T4, T5, T6) result;
	NEXTWEB_TUPLE_INIT_ITEMS6_(result, t1, t2, t3, t4, t5, t6);
	return result;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE7(T1, T2, T3, T4, T5, T6, T7)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3, T4 const &t4, T5 const &t5, T6 const &t6, T7 const &t7) {
	NEXTWEB_MAKE_TUPLE7(T1, T2, T3, T4, T5, T6, T7) result;
	NEXTWEB_TUPLE_INIT_ITEMS7_(result, t1, t2, t3, t4, t5, t6, t7);
	return result;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE8(T1, T2, T3, T4, T5, T6, T7, T8)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3, T4 const &t4, T5 const &t5, T6 const &t6, T7 const &t7, T8 const &t8) {
	NEXTWEB_MAKE_TUPLE8(T1, T2, T3, T4, T5, T6, T7, T8) result;
	NEXTWEB_TUPLE_INIT_ITEMS8_(result, t1, t2, t3, t4, t5, t6, t7, t8);
	return result;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9> NEXTWEB_INLINE NEXTWEB_MAKE_TUPLE9(T1, T2, T3, T4, T5, T6, T7, T8, T9)
makeTuple(T1 const &t1, T2 const &t2, T3 const &t3, T4 const &t4, T5 const &t5, T6 const &t6, T7 const &t7, T8 const &t8, T9 const &t9) {
	NEXTWEB_MAKE_TUPLE9(T1, T2, T3, T4, T5, T6, T7, T8, T9) result;
	NEXTWEB_TUPLE_INIT_ITEMS9_(result, t1, t2, t3, t4, t5, t6, t7, t8, t9);
	return result;
}

}}

#endif // NEXTWEB_TUPLE_UTILS_HPP_INCLUDED
