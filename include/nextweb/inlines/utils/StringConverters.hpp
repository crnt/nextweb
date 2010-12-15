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

#ifndef NEXTWEB_INLINES_UTILS_STRING_CONVERTERS_HPP_INCLUDED
#define NEXTWEB_INLINES_UTILS_STRING_CONVERTERS_HPP_INCLUDED

#include <cstdio>

#include "nextweb/Error.hpp"
#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace utils {

NEXTWEB_INLINE long long
CharArrayConverter<long long>::convert(char const *value) {
	long long retval;
	int result = sscanf(value, "%lld", &retval);
	SystemError::throwUnless(result >= 0);
	if (0 == result) {
		throw Error("no value to convert");
	}
	return retval;
}

NEXTWEB_INLINE unsigned long long
CharArrayConverter<unsigned long long>::convert(char const *value) {
	long long retval;
	int result = sscanf(value, "%llu", &retval);
	SystemError::throwUnless(result >= 0);
	if (0 == result) {
		throw Error("no value to convert");
	}
	return retval;
}

}} // namespaces

#endif // NEXTWEB_INLINES_UTILS_STRING_CONVERTERS_HPP_INCLUDED