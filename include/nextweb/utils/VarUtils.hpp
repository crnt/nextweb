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

#ifndef NEXTWEB_UTILS_VAR_UTILS_HPP_INCLUDED
#define NEXTWEB_UTILS_VAR_UTILS_HPP_INCLUDED

#include <string>
#include "nextweb/Error.hpp"

namespace nextweb { namespace utils {

template <typename Char>
struct VarConstants;

template <>
struct VarConstants<char> {
	
	static std::size_t endTokenSize();
	static char const* endToken();
	
	static std::size_t beginTokenSize();
	static char const* beginToken();
};

template <typename String, typename Map> String
substVars(String const &str, Map const &vars) {
	
	typedef typename String::value_type Char;
	
	typename String::size_type begin = str.find(VarConstants<Char>::beginToken());
	if (String::npos == begin) {
		return str;
	}
	typename String::size_type end = str.rfind(VarConstants<Char>::endToken());
	if (String::npos == end) {
		throw Error("bad variable syntax: %s", str.c_str());
	}
	
	typename String::size_type send = end - VarConstants<Char>::endTokenSize();
	typename String::size_type sbegin = begin + VarConstants<Char>::beginTokenSize();
	
	String res = substVars(String(str, sbegin, send - sbegin + 1), vars);
	typename Map::const_iterator i = vars.find(res);
	if (vars.end() == i) {
		throw Error("nonexistent variable: %s", res.c_str());
	}
	
	String copy(str);
	copy.replace(begin, end - begin + 1, i->second);
	return copy;
}

}} // namespaces

#endif // NEXTWEB_UTILS_VAR_UTILS_HPP_INCLUDED
