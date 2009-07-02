/** @file StringPred.hpp */
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

#ifndef NEXTWEB_DETAILS_STRING_PRED_HPP_INCLUDED
#define NEXTWEB_DETAILS_STRING_PRED_HPP_INCLUDED

#include <algorithm>

#include "details/Range.hpp"

namespace nextweb { namespace details {

template <typename Range, typename Other> inline bool
endsWithImpl(Range const &range, Other const &substr) {
	if (static_cast<std::size_t>(substr.size()) <= static_cast<std::size_t>(range.size())) {
		typename Range::const_iterator i = range.begin();
		std::advance(i, range.size() - substr.size());
		return std::equal(substr.begin(), substr.end(), i);
	}
	return false;
}

template <typename Range, typename Other, typename Pred> inline bool
endsWithImpl(Range const &range, Other const &substr, Pred pred) {
	if (static_cast<std::size_t>(substr.size()) <= static_cast<std::size_t>(range.size())) {
		typename Range::const_iterator i = range.begin();
		std::advance(i, range.size() - substr.size());
		return std::equal(substr.begin(), substr.end(), i, pred);
	}
	return false;
}

template <typename Range, typename Other> inline bool
startsWithImpl(Range const &range, Other const &substr) {
	if (static_cast<std::size_t>(substr.size()) <= static_cast<std::size_t>(range.size())) {
		return std::equal(substr.begin(), substr.end(), range.begin());
	}
	return false;
}

template <typename Range, typename Other, typename Pred> inline bool
startsWithImpl(Range const &range, Other const &substr, Pred pred) {
	if (static_cast<std::size_t>(substr.size()) <= static_cast<std::size_t>(range.size())) {
		return std::equal(substr.begin(), substr.end(), range.begin(), pred);
	}
	return false;
}

template <typename Range, typename Other> inline bool
endsWith(Range const &range, Other const &substr) {
	return endsWithImpl(makeRange(range), makeRange(substr));
}

template <typename Range, typename Other, typename Pred> inline bool
endsWith(Range const &range, Other const &substr, Pred pred) {
	return endsWithImpl(makeRange(range), makeRange(substr), pred);
}

template <typename Range, typename Other> inline bool
startsWith(Range const &range, Other const &substr) {
	return startsWithImpl(makeRange(range), makeRange(substr));
}

template <typename Range, typename Other, typename Pred> inline bool
startsWith(Range const &range, Other const &substr, Pred pred) {
	return startsWithImpl(makeRange(range), makeRange(substr), pred);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_STRING_PRED_HPP_INCLUDED
