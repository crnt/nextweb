/** @file MapUtils.hpp */
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

#ifndef NEXTWEB_DETAILS_MAP_UTILS_HPP_INCLUDED
#define NEXTWEB_DETAILS_MAP_UTILS_HPP_INCLUDED

namespace nextweb { namespace details {

template <typename Map> bool
has(Map const &map, typename Map::key_type const &name) {
 	return (map.end() != map.find(name));
}

template <typename Map> typename Map::mapped_type const&
get(Map const &map, typename Map::key_type const &name, typename Map::mapped_type const &value) {
	typename Map::const_iterator i = map.find(name);
	return (map.end() != i) ? i->second : value;
}

}} // namespaces

#endif // NEXTWEB_DETAILS_MAP_UTILS_HPP_INCLUDED
