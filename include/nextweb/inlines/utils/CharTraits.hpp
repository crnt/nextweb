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

#ifndef NEXTWEB_INLINES_UTILS_CHAR_TRAITS_HPP_INCLUDED
#define NEXTWEB_INLINES_UTILS_CHAR_TRAITS_HPP_INCLUDED

namespace nextweb { namespace utils {

NEXTWEB_INLINE bool
CharTraits<char>::isSpace(char value) {
	return (' ' == value) || ('\t' == value);
}

NEXTWEB_INLINE bool
CharTraits<char>::isAlpha(char value) {
	return isalpha(value);
}

NEXTWEB_INLINE bool
CharTraits<char>::isNumeric(char value) {
	return isdigit(value);
}

NEXTWEB_INLINE bool
CharTraits<char>::isAlphaNumeric(char value) {
	return isalnum(value);
}

NEXTWEB_INLINE char
CharTraits<char>::toUpper(char value) {
	return toupper(value);
}

NEXTWEB_INLINE char
CharTraits<char>::toLower(char value) {
	return tolower(value);
}

}} // namespaces

#endif // NEXTWEB_INLINES_UTILS_CHAR_TRAITS_HPP_INCLUDED
