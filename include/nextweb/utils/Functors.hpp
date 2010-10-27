// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@mail.ru>

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


#ifndef NEXTWEB_UTILS_FUNCTORS_HPP_INCLUDED
#define NEXTWEB_UTILS_FUNCTORS_HPP_INCLUDED

#include <algorithm>
#include <functional>

#include "nextweb/Config.hpp"
#include "nextweb/utils/CharTraits.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace utils {

template <typename Pred, typename T>
struct UnaryPredicate : public std::unary_function<T, bool> {
	bool operator () (T var) const;
};

template <typename Pred, typename T>
struct BinaryPredicate : public std::binary_function<T const&, T const&, bool> {
	bool operator () (T const &var, T const &target) const;
};

template <typename Char>
struct IsEqual : public std::unary_function<Char, bool> {
	IsEqual(Char pattern);
	bool operator () (Char value) const;
private:
	Char value_;
};

template <typename Char>
struct IsSpace : public UnaryPredicate<IsSpace<Char>, Char> {
	static bool check(Char value);
};

template <typename Char>
struct IsNumeric : public UnaryPredicate<IsNumeric<Char>, Char> {
	static bool check(Char value);
};

template <typename Char>
struct IsLineEnd : public UnaryPredicate<IsLineEnd<Char>, Char> {
	static bool check(Char value);
};

template <typename Char>
struct IsNotLineEnd : public UnaryPredicate<IsNotLineEnd<Char>, Char> {
	static bool check(Char value);
};

template <typename Sequence>
struct CILess : public BinaryPredicate<CILess<Sequence>, Sequence> {
	static bool check(Sequence const &var, Sequence const &target);
};

template <typename Sequence>
struct CIEqual : public BinaryPredicate<CIEqual<Sequence>, Sequence> {
	static bool check(Sequence const &var, Sequence const &target);
};

template <>
struct CILess<char> : public std::binary_function<char, char, bool> {
	bool operator () (char var, char target) const;
};

template <>
struct CIEqual<char> : public std::binary_function<char, char, bool> {
	bool operator () (char var, char target) const;
};

template <typename Pred, typename T> NEXTWEB_INLINE bool
UnaryPredicate<Pred, T>::operator () (T value) const {
	return Pred::check(value);
}

template <typename Pred, typename T> NEXTWEB_INLINE bool
BinaryPredicate<Pred, T>::operator () (T const &var, T const &target) const {
	return Pred::check(var, target);
}

template <typename Char> NEXTWEB_INLINE
IsEqual<Char>::IsEqual(Char value) :
	value_(value)
{
}

template <typename Char> NEXTWEB_INLINE bool
IsEqual<Char>::operator () (Char value) const {
	return value_ == value;
}

template <typename Char> NEXTWEB_INLINE bool
IsSpace<Char>::check(Char value) {
	return CharTraits<typename RemoveConst<Char>::Type>::isSpace(value);
}

template <typename Char> NEXTWEB_INLINE bool
IsNumeric<Char>::check(Char value) {
	return CharTraits<typename RemoveConst<Char>::Type>::isNumeric(value);
}

template <typename Char> NEXTWEB_INLINE bool
IsLineEnd<Char>::check(Char value) {
	return (static_cast<Char>('\n') == value || static_cast<Char>('\r') == value);
}

template <typename Char> NEXTWEB_INLINE bool
IsNotLineEnd<Char>::check(Char value) {
	return !IsLineEnd<Char>::check(value);
}

template <typename R1, typename R2> NEXTWEB_INLINE bool
isCILess(R1 const &var, R2 const &target) {
	return std::lexicographical_compare(var.begin(), var.end(), target.begin(), 
		target.end(), CILess<typename R1::value_type>());
}

template <typename R1, typename R2> NEXTWEB_INLINE bool
isCIEqual(R1 const &var, R2 const &target) {
	if (var.size() == target.size()) {
		return std::equal(var.begin(), var.end(), target.begin(), 
			CIEqual<typename R1::value_type>());
	}
	return false;
}

template <typename Sequence> NEXTWEB_INLINE bool
CILess<Sequence>::check(Sequence const &var, Sequence const &target) {
	return isCILess(var, target);
}

template <typename Sequence> NEXTWEB_INLINE bool
CIEqual<Sequence>::check(Sequence const &var, Sequence const &target) {
	return isCIEqual(var, target);
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/Functors.hpp"
#endif 

#endif // NEXTWEB_UTILS_FUNCTORS_HPP_INCLUDED
