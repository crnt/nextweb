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
#include <sstream>

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"

#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Integer.hpp"
#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace utils {

class ConvertError : public Error {
public:
	ConvertError();
};

template <typename X, typename String, bool IsInteger>
struct StringConverter;

template <typename X, typename String>
struct StringConverter<X, String, true>  {
	static String toString(X val);
};

template <typename X, typename String>
struct StringConverter<X, String, false>  {
	static String toString(X val);
	typedef std::basic_stringstream<typename String::value_type> StreamType;
};

template <typename String>
struct StringConverter<IntMax, String, true> {
	static String toString(IntMax val);
};

template <typename String>
struct StringConverter<UIntMax, String, true> {
	static String toString(UIntMax val);
};

template <typename X, typename String> NEXTWEB_INLINE String
StringConverter<X, String, true>::toString(X val) {
	typedef typename MaxInt<X>::Type ValueType;
	return StringConverter<ValueType, String, true>::toString(static_cast<ValueType>(val));
};

template <typename X, typename String> NEXTWEB_INLINE String
StringConverter<X, String, false>::toString(X val) {
	try {
		String res;
		StreamType stream;
		stream.exceptions(std::ios::badbit);
		stream << val;
		stream >> res;
		return res;
	}
	catch (std::ios::failure const &) {
		throw ConvertError();
	}
};

template <typename String> NEXTWEB_INLINE String
StringConverter<IntMax, String, true>::toString(IntMax val) {
	char buf[64];
	int res = snprintf(buf, sizeof(buf), "%lld", val);
	SystemError::throwUnless(res >= 0);
	Range<char const*> range(buf, buf + res);
	return String(range.begin(), range.end());
};

template <typename String> NEXTWEB_INLINE String
StringConverter<UIntMax, String, true>::toString(UIntMax val) {
	char buf[64];
	int res = snprintf(buf, sizeof(buf), "%llu", val);
	SystemError::throwUnless(res >= 0);
	Range<char const*> range(buf, buf + res);
	return String(range.begin(), range.end());
};

template <typename X> NEXTWEB_INLINE std::string
toString(X value) {
	return StringConverter<X, std::string, IsInt<X>::RESULT>::toString(value);
}

template <typename String, typename X> NEXTWEB_INLINE String
toString(X value) {
	return StringConverter<X, String, IsInt<X>::RESULT>::toString(value);
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/StringConverters.hpp"
#endif

#endif // NEXTWEB_UTILS_STRING_CONVERTERS_HPP_INCLUDED
