/** @file functors.hpp */
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


#ifndef NEXTWEB_DETAILS_FUNCTORS_HPP_INCLUDED
#define NEXTWEB_DETAILS_FUNCTORS_HPP_INCLUDED

#include <functional>

#include "details/CharTraits.hpp"

namespace nextweb { namespace details {

template <typename Pred, typename T>
struct UnaryPredicate : public std::unary_function<T, bool> {
	bool operator () (T var) const;
};

template <typename Pred, typename T>
struct BinaryPredicate : public std::binary_function<T const&, T const&, bool> {
	bool operator () (T const &var, T const &target) const;
};

template <typename Char>
struct IsSpace : public UnaryPredicate<IsSpace<Char>, Char> {
	static bool check(Char value);
};

template <typename Char>
struct IsAlpha : public UnaryPredicate<IsAlpha<Char>, Char> {
	static bool check(Char value);
};

template <typename Char>
struct IsLineEnd : public UnaryPredicate<IsLineEnd<Char>, Char> {
	static bool check(Char value);
};

template <typename Char>
struct IsAlphaNumeric : public UnaryPredicate<IsAlphaNumeric<Char>, Char> {
	static bool check(Char value);
};

template <typename Range>
struct CILess : public BinaryPredicate<CILess<Range>, Range> {
	static bool check(Range const &var, Range const &target);
};

template <typename Range>
struct CIEqual : public BinaryPredicate<CIEqual<Range>, Range> {
	static bool check(Range const &var, Range const &target);
};

template <>
struct CILess<char> : public std::binary_function<char, char, bool> {
	bool operator () (char var, char target) const;
};

template <>
struct CIEqual<char> : public std::binary_function<char, char, bool> {
	bool operator () (char var, char target) const;
};

template <typename Pred, typename T> inline bool
UnaryPredicate<Pred, T>::operator () (T value) const {
	return Pred::check(value);
}

template <typename Pred, typename T> inline bool
BinaryPredicate<Pred, T>::operator () (T const &var, T const &target) const {
	return Pred::check(var, target);
}

template <typename Char> inline bool
IsSpace<Char>::check(Char value) {
	return CharTraits<Char>::isSpace(value);
}

template <typename Char> inline bool
IsAlpha<Char>::check(Char value) {
	return CharTraits<Char>::isAlpha(value);
}

template <typename Char> inline bool
IsLineEnd<Char>::check(Char value) {
	return (static_cast<Char>('\n') == value || static_cast<Char>('\r') == value);
}

template <typename Char> inline bool
IsAlphaNumeric<Char>::check(Char value) {
	return CharTraits<Char>::isAlphaNumeric(value);
}

template <typename Range> inline bool
CILess<Range>::check(Range const &var, Range const &target) {
	return std::lexicographical_compare(var.begin(), var.end(), target.begin(), 
		target.end(), CILess<typename Range::value_type>());
}

template <typename Range> inline bool
CIEqual<Range>::check(Range const &var, Range const &target) {
	if (var.size() == target.size()) {
		return std::equal(var.begin(), var.end(), target.begin(), 
			CIEqual<typename Range::value_type>());
	}
	return false;
}

inline bool
CILess<char>::operator () (char var, char target) const {
	return CharTraits<char>::toLower(var) < CharTraits<char>::toLower(target);
}

inline bool
CIEqual<char>::operator () (char var, char target) const {
	return CharTraits<char>::toLower(var) == CharTraits<char>::toLower(target);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_FUNCTORS_HPP_INCLUDED
