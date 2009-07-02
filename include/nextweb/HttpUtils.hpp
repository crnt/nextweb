/** @file HttpUtils.hpp */
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

#ifndef NEXTWEB_HTTP_UTILS_HPP_INCLUDED
#define NEXTWEB_HTTP_UTILS_HPP_INCLUDED

#include <ctime>
#include <cstring>
#include <cstddef>

#include "nextweb/Config.hpp"
#include "nextweb/StandardSetup.hpp"

namespace nextweb {

NEXTWEB_API std::time_t
parseHttpDate(char const *buffer);

NEXTWEB_API void
printHttpDate(char *buffer, std::size_t size, std::time_t what);

template <typename String> NEXTWEB_API inline String
printHttpDate(std::size_t what) {
	char buffer[256];
	printHttpDate(buffer, sizeof(buffer), what);
	return String(buffer, buffer + strlen(buffer));
}

NEXTWEB_API StandardSetup::StringType
urlencode(StandardSetup::StringType const &value);

NEXTWEB_API StandardSetup::StringType
urldecode(StandardSetup::StringType const &value);

} // namespace

#endif // NEXTWEB_HTTP_UTILS_HPP_INCLUDED
