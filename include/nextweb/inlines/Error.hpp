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

#ifndef NEXTWEB_INLINES_ERROR_HPP_INCLUDED
#define NEXTWEB_INLINES_ERROR_HPP_INCLUDED

#include <cstdio>
#include <cstdarg>
#include <cstring>

namespace nextweb {

NEXTWEB_INLINE
Error::Error(char const *format, ...) :
	std::exception()
{
	va_list args;
	va_start(args, format);
	vsnprintf(message_, MESSAGE_SIZE, format, args);
	va_end(args);
}

NEXTWEB_INLINE
Error::~Error() throw () {
}

NEXTWEB_INLINE
Error::Error(Error const &other) :
	std::exception(other)
{
	memcpy(message_, other.message_, MESSAGE_SIZE);
}

NEXTWEB_INLINE Error&
Error::operator = (Error const &other) {
	memcpy(message_, other.message_, MESSAGE_SIZE);
	return *this;
}
	
NEXTWEB_INLINE char const*
Error::what() const throw () {
	return message_;
}

} // namespace

#endif // NEXTWEB_INLINES_ERROR_HPP_INCLUDED
