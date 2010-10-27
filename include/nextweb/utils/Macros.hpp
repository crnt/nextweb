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

#ifndef NEXTWEB_UTILS_MACROS_HPP_INCLUDED
#define NEXTWEB_UTILS_MACROS_HPP_INCLUDED

#define NEXTWEB_CONCAT(x, y) NEXTWEB_DO_CONCAT(x, y)
#define NEXTWEB_DO_CONCAT(x, y) NEXTWEB_PROCESS_CONCAT(x, y)
#define NEXTWEB_PROCESS_CONCAT(x, y) x##y
#define NEXTWEB_MAKE_UNIQUE_NAME(prefix) NEXTWEB_CONCAT(prefix, __LINE__)

#endif // NEXTWEB_UTILS_MACROS_HPP_INCLUDED
