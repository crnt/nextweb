/** @file StringConverters.hpp */
// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@nextweb.org>

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

#ifndef NEXTWEB_DETAILS_STRING_CONVERTERS_HPP_INCLUDED
#define NEXTWEB_DETAILS_STRING_CONVERTERS_HPP_INCLUDED

#include <cstdio>
#include <limits>

#include "nextweb/Error.hpp"
#include "details/SystemError.hpp"

namespace nextweb { namespace details {

template <typename Setup, typename Basic>
struct StringConverter {
	static typename Setup::StringType toString(Basic value);
	static Basic fromString(typename Setup::StringType const &value);
};

template <typename Setup>
struct StringConverter<Setup, long long> {
	typedef long long BasicType;
	static typename Setup::StringType toString(long long value);
	static long long fromString(typename Setup::StringType const &value);
};

template <typename Setup>
struct StringConverter<Setup, unsigned long long> {
	typedef unsigned long long BasicType;
	static typename Setup::StringType toString(unsigned long long value);
	static unsigned long long fromString(typename Setup::StringType const &value);
};

template <typename Setup, bool IsSigned>
struct StringConverterSelector;

template <typename Setup>
struct StringConverterSelector<Setup, true> {
	typedef StringConverter<Setup, long long> Type;
};

template <typename Setup>
struct StringConverterSelector<Setup, false> {
	typedef StringConverter<Setup, unsigned long long> Type;
};

template <typename Setup, typename Basic> inline typename Setup::StringType
StringConverter<Setup, Basic>::toString(Basic value) {
	typedef typename StringConverterSelector<Setup, std::numeric_limits<Basic>::is_signed>::Type ConverterType;
	return ConverterType::toString(static_cast<typename ConverterType::BasicType>(value));
}

template <typename Setup, typename Basic> inline Basic
StringConverter<Setup, Basic>::fromString(typename Setup::StringType const &value) {
	typedef typename StringConverterSelector<Setup, std::numeric_limits<Basic>::is_signed>::Type ConverterType;
	typename ConverterType::BasicType result = ConverterType::fromString(value);
	if (static_cast<typename ConverterType::BasicType>(std::numeric_limits<Basic>::max()) < result) {
		throw Error("value too big");
	}
	return static_cast<Basic>(result);
}

template <typename Setup> inline typename Setup::StringType
StringConverter<Setup, long long>::toString(long long value) {
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%lld", value);
	return typename Setup::StringType(buffer);
}

template <typename Setup> inline long long
StringConverter<Setup, long long>::fromString(typename Setup::StringType const &value) {
	long long retval;
	int result = sscanf(value.c_str(), "%lld", &retval);
	SystemError::throwUnless(result >= 0);
	if (0 == result) {
		throw Error("no value to convert");
	}
	return retval;
}

template <typename Setup> inline typename Setup::StringType
StringConverter<Setup, unsigned long long>::toString(unsigned long long value) {
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%llu", value);
	return typename Setup::StringType(buffer);
}

template <typename Setup> inline unsigned long long
StringConverter<Setup, unsigned long long>::fromString(typename Setup::StringType const &value) {
	unsigned long long retval;
	int result = sscanf(value.c_str(), "%llu", &retval);
	SystemError::throwUnless(result >= 0);
	if (0 == result) {
		throw Error("no value to convert");
	}
	return retval;
}

template <typename Setup, typename Basic> typename Setup::StringType
toString(Basic value) {
	return StringConverter<Setup, Basic>::toString(value);
}

template <typename Setup, typename Basic> Basic
fromString(typename Setup::StringType const &value) {
	return StringConverter<Setup, Basic>::fromString(value);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_STRING_CONVERTERS_HPP_INCLUDED
