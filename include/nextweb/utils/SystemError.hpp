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

#ifndef NEXTWEB_UTILS_SYSTEM_ERROR_HPP_INCLUDED
#define NEXTWEB_UTILS_SYSTEM_ERROR_HPP_INCLUDED

#include "nextweb/Error.hpp"

namespace nextweb { namespace utils {

class SystemError : public Error {

public:
	SystemError(char const *msg);
	static void throwError(int error);
	static void throwUnless(bool condition);
};

}} // namespaces

#endif // NEXTWEB_UTILS_SYSTEM_ERROR_HPP_INCLUDED
