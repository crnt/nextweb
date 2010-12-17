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

#ifndef NEXTWEB_UTILS_STRING_CONVERTERS_HPP_INCLUDED
#define NEXTWEB_UTILS_STRING_CONVERTERS_HPP_INCLUDED

#include <cstdio>
#include <limits>
#include <string>

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"

#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/SystemError.hpp"
#include "nextweb/utils/StaticAssert.hpp"

namespace nextweb { namespace utils {

template <bool IsSigned>
struct MaxTypeToConvert;

template <>
struct MaxTypeToConvert<true> {
	typedef long long Type;
};

template <>
struct MaxTypeToConvert<false> {
	typedef unsigned long long Type;
};

template <typename X>
struct CharArrayConverter;

template <>
struct CharArrayConverter<long long> {
	typedef long long ValueType;
	long long convert(char const *value);
};

template <>
struct CharArrayConverter<unsigned long long> {
	typedef unsigned long long ValueType;
	unsigned long long convert(char const *value);
};

template <typename String, typename X>
struct StringConverter {
	static String toString(X value);
	static X fromString(String const &value);
	NEXTWEB_STATIC_ASSERT((IsSame<typename String::value_type, char>::RESULT));
};

template <typename String>
struct StringConverter<String, long long> {
	typedef long long ValueType;
	static String toString(long long value);
	static long long fromString(String const &value);
	NEXTWEB_STATIC_ASSERT((IsSame<typename String::value_type, char>::RESULT));
};

template <typename String>
struct StringConverter<String, unsigned long long> {
	typedef unsigned long long ValueType;
	static String toString(unsigned long long value);
	static unsigned long long fromString(String const &value);
	NEXTWEB_STATIC_ASSERT((IsSame<typename String::value_type, char>::RESULT));
};

template <typename String, bool IsSigned>
struct StringConverterSelector;

template <typename String>
struct StringConverterSelector<String, true> {
	typedef StringConverter<String, long long> Type;
};

template <typename String>
struct StringConverterSelector<String, false> {
	typedef StringConverter<String, unsigned long long> Type;
};

template <typename String, typename X> NEXTWEB_INLINE String
StringConverter<String, X>::toString(X value) {
	typedef typename StringConverterSelector<String, std::numeric_limits<X>::is_signed>::Type ConverterType;
	return ConverterType::toString(static_cast<typename ConverterType::ValueType>(value));
}

template <typename String, typename X> NEXTWEB_INLINE X
StringConverter<String, X>::fromString(String const &value) {
	typedef typename StringConverterSelector<String, std::numeric_limits<X>::is_signed>::Type ConverterType;
	typename ConverterType::ValueType result = ConverterType::fromString(value);
	if (static_cast<typename ConverterType::ValueType>(std::numeric_limits<X>::max()) < result) {
		throw Error("value too big");
	}
	return static_cast<X>(result);
}

template <typename String> NEXTWEB_INLINE String
StringConverter<String, long long>::toString(long long value) {
	char buffer[64];
	int length = snprintf(buffer, sizeof(buffer), "%lld", value);
	SystemError::throwUnless(length >= 0);
	Range<char const*> range(buffer, buffer + length);
	return String(range.begin(), range.end());
}

template <typename String> NEXTWEB_INLINE long long
StringConverter<String, long long>::fromString(String const &value) {
	return CharArrayConverter<long long>::convert(value.c_str());
}

template <typename String> NEXTWEB_INLINE String
StringConverter<String, unsigned long long>::toString(unsigned long long value) {
	char buffer[64];
	int length = snprintf(buffer, sizeof(buffer), "%llu", value);
	SystemError::throwUnless(length >= 0);
	Range<char const*> range(buffer, buffer + length);
	return String(range.begin(), range.end());
}

template <typename String> NEXTWEB_INLINE unsigned long long
StringConverter<String, unsigned long long>::fromString(String const &value) {
	return CharArrayConverter<unsigned long long>::convert(value.c_str());
}

template <typename String> NEXTWEB_INLINE String
toString(char const *value) {
	return String(value);
}

template <typename X> NEXTWEB_INLINE X
fromString(char const *value) {
	typedef CharArrayConverter<typename MaxTypeToConvert<std::numeric_limits<X>::is_signed>::Type> ConverterType;
	typename ConverterType::ValueType result = ConverterType::convert(value);
	if (static_cast<typename ConverterType::ValueType>(std::numeric_limits<X>::max()) < result) {
		throw Error("value too big");
	}
	return static_cast<X>(result);
}

template <typename X> NEXTWEB_INLINE std::string
toString(X const &value) {
	return StringConverter<std::string, X>::toString(value);
}

template <typename String, typename X> NEXTWEB_INLINE String
toString(X const &value) {
	return StringConverter<String, X>::toString(value);
}

template <typename X, typename String> NEXTWEB_INLINE X
fromString(String const &value) {
	return StringConverter<String, X>::fromString(value);
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/StringConverters.hpp"
#endif

#endif // NEXTWEB_UTILS_STRING_CONVERTERS_HPP_INCLUDED
