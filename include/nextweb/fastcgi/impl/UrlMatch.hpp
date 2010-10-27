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

#ifndef NEXTWEB_FASTCGI_URL_MATCH_HPP_INCLUDED
#define NEXTWEB_FASTCGI_URL_MATCH_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <algorithm>
#include <functional>

#include "nextweb/Config.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Tokenizer.hpp"

namespace nextweb { namespace fastcgi {

struct MatchConstants {
	static utils::Range<char const*> const STAR;
};

template <typename Sequence> NEXTWEB_INLINE std::pair<bool, std::size_t> 
urlMatch(Sequence const &seq, Sequence const &pattern) {

	using namespace utils;
	
	typedef SplitIfEqual<typename Sequence::value_type> Matcher;
	typedef Range<typename Sequence::const_reverse_iterator> Reversed;
	
	Matcher matcher(static_cast<typename Sequence::value_type>('/'));
	Tokenizer<Reversed, Matcher> tseq(Reversed(seq.rbegin(), seq.rend()), matcher);
	Tokenizer<Reversed, Matcher> tpat(Reversed(pattern.rbegin(), pattern.rend()), matcher);
	
	std::pair<bool, std::size_t> result(true, 0);
	typename Tokenizer<Reversed, Matcher>::const_iterator si = tseq.begin(), send = tseq.end(), pi = tpat.begin();
	for (std::size_t count = 0 ; si != send; ++count, ++si, ++pi) {
		if (MatchConstants::STAR == *pi) {
			result.second += (count + 1);
		}
		else if (*pi != *si) {
			result.first = false;
			break;
		}
	}
	return result;
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_URL_MATCH_HPP_INCLUDED
