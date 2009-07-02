/** @file StringUtils.hpp */
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

#ifndef NEXTWEB_DETAILS_STRING_UTILS_HPP_INCLUDED
#define NEXTWEB_DETAILS_STRING_UTILS_HPP_INCLUDED

#include "details/Functors.hpp"
#include "details/StringPred.hpp"
#include "details/UpperLower.hpp"
#include "details/StringSplit.hpp"
#include "details/StringConverters.hpp"

namespace nextweb { namespace details {

template <typename Iter, typename Pred> inline Iter
nextMatched(Iter begin, Iter end, Pred pred) {
	return std::find_if(begin, end, pred);
}

template <typename Iter, typename Pred> inline Iter
nextNotMatched(Iter begin, Iter end, Pred pred) {
	for (Iter i = begin; i != end; ++i) {
		if (!pred(*i)) return i;
	}
	return end;
}

template <typename Range> inline Range
trimLeft(Range const &range) {
	IsSpace<typename Range::value_type> checker;
	return Range(nextNotMatched(range.begin(), range.end(), checker), range.end());
}

template <typename Range> inline Range
trimRight(Range const &range) {
	IsSpace<typename Range::value_type> checker;
	return Range(range.begin(), nextNotMatched(range.rbegin(), range.rend(), checker).base());
}

template <typename Range> inline Range
trim(Range const &range) {
	return trimLeft(trimRight(range));
}

template<typename Range> inline Range
truncate(Range const &range, typename Range::size_type left, typename Range::size_type right) {
	typename Range::const_iterator begin = range.begin(), end = range.end();
	while (begin != end && left--) {
		++begin;
	}
	while (begin != end && right--) {
		--end;
	}
	return Range(begin, end);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_STRING_UTILS_HPP_INCLUDED
