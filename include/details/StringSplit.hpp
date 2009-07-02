/** @file StringSplit.hpp */
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

#ifndef NEXTWEB_DETAILS_STRING_SPLIT_HPP_INCLUDED
#define NEXTWEB_DETAILS_STRING_SPLIT_HPP_INCLUDED

#include <algorithm>

#include "details/Range.hpp"

namespace nextweb { namespace details {

template <typename Range> inline bool
splitImpl(const Range &range, typename Range::const_iterator pos,
	typename Range::size_type size, Range &first, Range &second) 
{
	typename Range::const_iterator begin = range.begin(), end = range.end();
	first = Range(begin, pos);
	if (pos != end) {
		second = Range(pos + size, end);
		return true;
	}
	else {
		second = Range(end, end);
		return false;
	}
}

template <typename Range, typename Delim> inline bool
splitImpl(Range const &range, Delim const &delim, Range &first, Range &second) {
	return splitImpl(range, std::search(range.begin(), range.end(), delim.begin(), delim.end()), delim.size(), first, second);
}

template <typename Range, typename Other> inline bool
splitFirstOfImpl(Range const &range, Other const &other, Range &first, Range &second) {
	return splitImpl(range, std::find_first_of(range.begin(), range.end(), 
		other.begin(), other.end()), 1, first, second);
}

template <typename Range> inline bool
split(Range const &range, char c, Range &first, Range &second) {
	return splitImpl(range, std::find(range.begin(), range.end(), c), 1, first, second);
}

template <typename Range, typename Delim> inline bool
split(Range const &range, Delim const &delim, Range &first, Range &second) {
	return splitImpl(range, makeRange(delim), first, second);
}

template <typename Range, typename Other> inline bool
splitFirstOf(Range const &range, Other const &other, Range &first, Range &second) {
	return splitFirstOfImpl(range, makeRange(other), first, second);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_STRING_SPLIT_HPP_INCLUDED
