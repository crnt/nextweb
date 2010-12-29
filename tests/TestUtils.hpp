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

#include "nextweb/Config.hpp"

namespace nextweb { namespace tests {

template <typename Sequence> NEXTWEB_INLINE Sequence
as(char *str) {
	return Sequence(str, str + std::char_traits<char>::length(str));
}

template <typename Sequence> NEXTWEB_INLINE Sequence
as(char const *str) {
	return Sequence(str, str + std::char_traits<char>::length(str));
}

template <typename Sequence> NEXTWEB_INLINE std::string
makeString(Sequence const &seq) {
	return std::string(seq.begin(), seq.end());
}

template <typename Iter> NEXTWEB_INLINE std::size_t
hashSum(Iter begin, Iter end) {
	std::size_t result = 0;
	for (; begin  != end; ++begin) {
		result += (*begin) * 5;
	}
	return result;
}

}} // namespaces

#endif // NEXTWEB_TESTS_TEST_UTILS_HPP_INCLUDED
