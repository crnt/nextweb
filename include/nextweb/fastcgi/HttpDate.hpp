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

#ifndef NEXTWEB_FASTCGI_HTTP_DATE_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HTTP_DATE_HPP_INCLUDED

#include <ctime>
#include <string>
#include <iostream>

#include "nextweb/Config.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API HttpDate {

public:
	HttpDate();
	explicit HttpDate(std::time_t t);
	virtual ~HttpDate();

	HttpDate(HttpDate const &other);
	HttpDate& operator = (HttpDate const &other);

	void swap(HttpDate &other) throw ();

	static HttpDate fromString(char const *str);
	static HttpDate fromString(std::string const &str);

	static HttpDate fromPeriod(char const *period);
	static HttpDate fromPeriod(std::string const &period);

	std::string str() const;
	std::string asRFC1123() const;
	std::string asRFC1036() const;
	std::string asAsctime() const;
	
	HttpDate& operator += (char const *value);
	HttpDate& operator -= (char const *value);
	
	bool operator == (HttpDate const &other) const;
	bool operator != (HttpDate const &other) const;

	static HttpDate const BAD;

private:
	HttpDate& add(char const *period);
	HttpDate& sub(char const *period);

private:
	std::time_t when_;
};

HttpDate operator + (HttpDate const &source, char const *value);
HttpDate operator + (char const *value, HttpDate const &source);
HttpDate operator - (HttpDate const &source, char const *value);

template <typename String> NEXTWEB_INLINE HttpDate
operator + (HttpDate const &source, String const &value) {
	return source + value.c_str();
}

template <typename String> NEXTWEB_INLINE HttpDate
operator + (String const &value, HttpDate const &source) {
	return source + value.c_str();
}

template <typename String> NEXTWEB_INLINE HttpDate
operator - (HttpDate const &source, String const &value) {
	return source - value.c_str();
}

template <typename Char, typename Traits> NEXTWEB_INLINE std::basic_ostream<Char, Traits>&
operator << (std::basic_ostream<Char, Traits> &stream, HttpDate const &date) {
	stream << date.str();
	return stream;
}

NEXTWEB_API void
swap(HttpDate &date, HttpDate &other);

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/fastcgi/HttpDate.hpp"
#endif

#endif // NEXTWEB_FASTCGI_HTTP_DATE_HPP_INCLUDED
