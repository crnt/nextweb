/** @file CharTraits.hpp */
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

#ifndef NEXTWEB_DETAILS_CHAR_TRAITS_HPP_INCLUDED
#define NEXTWEB_DETAILS_CHAR_TRAITS_HPP_INCLUDED

#include <cctype>

namespace nextweb { namespace details {

template <typename Char>
struct CharTraits;

template <>
struct CharTraits<char> {
	
	static bool isSpace(char value);
	static bool isAlpha(char value);
	static bool isAlphaNumeric(char value);

	static char toUpper(char value);
	static char toLower(char value);
};

inline bool
CharTraits<char>::isSpace(char value) {
	return isspace(value);
}

inline bool
CharTraits<char>::isAlpha(char value) {
	return isalpha(value);
}

inline bool
CharTraits<char>::isAlphaNumeric(char value) {
	return isalnum(value);
}

inline char
CharTraits<char>::toUpper(char value) {
	return toupper(value);
}

inline char
CharTraits<char>::toLower(char value) {
	return tolower(value);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_CHAR_TRAITS_HPP_INCLUDED
