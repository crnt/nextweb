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

#ifndef NEXTWEB_INLINES_FASTCGI_HTTP_DATE_HPP_INCLUDED
#define NEXTWEB_INLINES_FASTCGI_HTTP_DATE_HPP_INCLUDED

namespace nextweb { namespace fastcgi {

NEXTWEB_INLINE bool
HttpDate::operator == (HttpDate const &other) const {
	return when_ == other.when_;
}

NEXTWEB_INLINE bool
HttpDate::operator != (HttpDate const &other) const {
	return when_ != other.when_;
}

NEXTWEB_API NEXTWEB_INLINE HttpDate&
HttpDate::operator += (char const *value) {
	add(value);
	return *this;
}

NEXTWEB_API NEXTWEB_INLINE HttpDate&
HttpDate::operator -= (char const *value) {
	sub(value);
	return *this;
}

NEXTWEB_API NEXTWEB_INLINE HttpDate
operator + (HttpDate const &source, char const *value) {
	HttpDate copy(source);
	copy += value;
	return copy;
}

NEXTWEB_API NEXTWEB_INLINE HttpDate
operator + (char const *value, HttpDate const &source) {
	return source + value;
}

NEXTWEB_API NEXTWEB_INLINE HttpDate
operator - (HttpDate const &source, char const *value) {
	HttpDate copy(source);
	copy -= value;
	return copy;
}

NEXTWEB_API NEXTWEB_INLINE void
swap(HttpDate &date, HttpDate &other) {
	date.swap(other);
}

}} // namespaces

#endif // NEXTWEB_INLINES_FASTCGI_HTTP_DATE_HPP_INCLUDED
