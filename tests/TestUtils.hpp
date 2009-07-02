/** @file TestUtils.hpp */
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

#ifndef NEXTWEB_TESTS_TEST_UTILS_HPP_INCLUDED
#define NEXTWEB_TESTS_TEST_UTILS_HPP_INCLUDED

#include <string>

#include "details/Range.hpp"

namespace nextweb { namespace tests {

template <typename Cont> inline Cont
as(const char *str) {
	return Cont(str, str + strlen(str));
}

template <typename Cont> inline std::string
makeString(Cont const &cont) {
	return std::string(cont.begin(), cont.end());
}

inline std::string
makeString(details::Range<char const*> const &range) {
	return std::string(range.begin(), range.size());
}

}} // namespaces

#endif // NEXTWEB_TESTS_TEST_UTILS_HPP_INCLUDED
