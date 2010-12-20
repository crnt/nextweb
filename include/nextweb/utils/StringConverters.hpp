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

namespace nextweb { namespace utils {

class ConvertError : public Error {
public:
	ConvertError();
	static void throwUnless(bool condition);
};

template <bool IsSigned>
struct IntConverter;

template <>
struct IntConverter<true> {
	static IntMax fromCharPtr(char const *value);
};

template <>
struct IntConverter<false> {
	static UIntMax fromCharPtr(char const *value);
};

template <typename X, bool IsInteger>
struct CharPtrConverter;

template <typename X>
struct CharPtrConverter<X, true> {
	static X fromCharPtr(char const *val);
};

template <typename X>
struct CharPtrConverter<X, false> {
	static X fromCharPtr(char const *val);
};

template <typename X, typename String, bool IsInteger>
struct StringConverter;

template <typename X, typename String>
struct StringConverter<X, String, true>  {
	static String toString(X val);
	static X fromString(String const &val);
};

template <typename X, typename String>
struct StringConverter<X, String, false>  {
	static String toString(X val);
	static X fromString(String const &val);
	typedef std::basic_stringstream<typename String::value_type> StreamType;
};

template <typename String>
struct StringConverter<String, String, false> {
	static String toString(String const &val);
	static String fromString(String const &val);
};

template <typename String>
struct StringConverter<IntMax, String, true> {
	static String toString(IntMax val);
	static IntMax fromString(String const &val);
};

template <typename String>
struct StringConverter<UIntMax, String, true> {
	static String toString(UIntMax val);
	static UIntMax fromString(String const &val);
};

template <typename X> X
fromCharPtr(char const *value);

template <typename X> NEXTWEB_INLINE X
CharPtrConverter<X, true>::fromCharPtr(char const *val) {
	typedef typename MaxInt<X>::Type IntType;
	IntType res = IntConverter<std::numeric_limits<X>::is_signed>::fromCharPtr(val);
	if (static_cast<IntType>(std::numeric_limits<X>::max()) < res) {
		throw ConvertError();
	}
	return res;
}

template <typename X> NEXTWEB_INLINE X
CharPtrConverter<X, false>::fromCharPtr(char const *val) {
	try {
		X res;
		std::stringstream stream;
		stream.exceptions(std::ios::failbit);
		stream << val;
		stream >> res;
		return res;
	}
	catch (std::ios::failure const &) {
		throw ConvertError();
	}
}

template <typename X, typename String> NEXTWEB_INLINE String
StringConverter<X, String, true>::toString(X val) {
	typedef typename MaxInt<X>::Type ValueType;
	return StringConverter<ValueType, String, true>::toString(static_cast<ValueType>(val));
};

template <typename X, typename String> NEXTWEB_INLINE X
StringConverter<X, String, true>::fromString(String const &val) {
	return fromCharPtr<X>(val.c_str());
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

template <typename X, typename String> NEXTWEB_INLINE X
StringConverter<X, String, false>::fromString(String const &val) {
	try {
		X res;
		StreamType stream;
		stream.exceptions(std::ios::badbit);
		stream << val;
		stream >> res;
		return res;
	}
	catch (std::ios::failure const &) {
		throw ConvertError();
	}
}

template <typename String> NEXTWEB_INLINE String
StringConverter<String, String, false>::toString(String const &val) {
	return val;
}

template <typename String> NEXTWEB_INLINE String
StringConverter<String, String, false>::fromString(String const &val) {
	return val;
}

template <typename String> NEXTWEB_INLINE String
StringConverter<IntMax, String, true>::toString(IntMax val) {
	char buf[64];
	int success = snprintf(buf, sizeof(buf), "%lld", val);
	ConvertError::throwUnless(success > 0);
	Range<char const*> range(buf, buf + success);
	return String(range.begin(), range.end());
};

template <typename String> NEXTWEB_INLINE IntMax
StringConverter<IntMax, String, true>::fromString(String const &val) {
	return fromCharPtr<IntMax>(val.c_str());
}

template <typename String> NEXTWEB_INLINE UIntMax
StringConverter<UIntMax, String, true>::fromString(String const &val) {
	return fromCharPtr<UIntMax>(val.c_str());
}

template <typename String> NEXTWEB_INLINE String
StringConverter<UIntMax, String, true>::toString(UIntMax val) {
	char buf[64];
	int success = snprintf(buf, sizeof(buf), "%llu", val);
	ConvertError::throwUnless(success > 0);
	Range<char const*> range(buf, buf + success);
	return String(range.begin(), range.end());
};

template <typename X> NEXTWEB_INLINE X
fromCharPtr(char const *val) {
	return CharPtrConverter<X, IsInt<X>::RESULT>::fromCharPtr(val);
}

template <typename X> NEXTWEB_INLINE std::string
toString(X value) {
	return StringConverter<X, std::string, IsInt<X>::RESULT>::toString(value);
}

template <typename String, typename X> NEXTWEB_INLINE String
toString(X value) {
	return StringConverter<X, String, IsInt<X>::RESULT>::toString(value);
}

template <typename X> NEXTWEB_INLINE X
fromString(char const *val) {
	return fromCharPtr<X>(val);
}

template <typename X, typename String> NEXTWEB_INLINE X
fromString(String const &val) {
	return StringConverter<X, String, IsInt<X>::RESULT>::fromString(val);
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/StringConverters.hpp"
#endif

#endif // NEXTWEB_UTILS_STRING_CONVERTERS_HPP_INCLUDED
