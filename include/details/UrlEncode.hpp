/** @file UrlEncode.hpp */
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

#ifndef NEXTWEB_DETAILS_URL_ENCODE_HPP_INCLUDED
#define NEXTWEB_DETAILS_URL_ENCODE_HPP_INCLUDED

#include <string>
#include <cstddef>
#include <iterator>

#include "details/CharTraits.hpp"

namespace nextweb { namespace details {

template <typename Result, typename Range, std::size_t N>
class UrlEncoder;

template <typename Result, typename Range>
struct UrlEncoder<Result, Range, static_cast<std::size_t>(1)> {
	static Result encode(Range const &range);
	static Result decode(Range const &range);
};

template <typename Range> inline Range
urlencode(Range const &range) {
	return UrlEncoder<Range, Range, sizeof(typename Range::value_type)>::encode(range);
}

template <typename Result, typename Range> inline Result
urlencode(Range const &range) {
	return UrlEncoder<Result, Range, sizeof(typename Range::value_type)>::encode(range);
}

template <typename Range> inline Range
urldecode(Range const &range) {
	return UrlEncoder<Range, Range, sizeof(typename Range::value_type)>::decode(range);
}

template <typename Result, typename Range> inline Result
urldecode(Range const &range) {
	return UrlEncoder<Result, Range, sizeof(typename Range::value_type)>::decode(range);
}

template <typename Result, typename Range> inline Result
UrlEncoder<Result, Range, static_cast<std::size_t>(1)>::encode(Range const &range) {
	
	Result result;
	result.reserve(3 * range.size());
	typedef typename Range::value_type CharType;
	
	for (typename Range::const_iterator i = range.begin(), end = range.end(); i != end; ++i) {
		if (CharTraits<CharType>::isAlphaNumeric(*i)) {
			result.push_back(*i);
			continue;
		}
		switch (*i) {
			case static_cast<CharType>('-'): 
			case static_cast<CharType>('_'):
			case static_cast<CharType>('.'):
			case static_cast<CharType>('!'):
			case static_cast<CharType>('~'): 
			case static_cast<CharType>('*'):
			case static_cast<CharType>('('):
			case static_cast<CharType>(')'):
			case static_cast<CharType>('\''): 
				result.push_back(*i);
				break;
			default:
				result.push_back(static_cast<CharType>('%'));
				CharType bytes[3] = { 0, 0, 0 };
				bytes[0] = (*i & 0xF0) / 16 ;
				bytes[0] += (bytes[0] > 9) ? 'A' - 10 : '0';
				bytes[1] = (*i & 0x0F);
				bytes[1] += (bytes[1] > 9) ? 'A' - 10 : '0';
				result.insert(result.end(), bytes, bytes + sizeof(bytes) - 1);
				break;
		}
	}
	return result;
}

template <typename Result, typename Range> inline Result
UrlEncoder<Result, Range, static_cast<std::size_t>(1)>::decode(Range const &range) {
	
	Result result;
	result.reserve(range.size());
	typedef typename Range::value_type CharType;
	
	for (typename Range::const_iterator i = range.begin(), end = range.end(); i != end; ++i) {
		switch (*i) {
			case static_cast<CharType>('+'): 
				result.push_back(static_cast<CharType>(' '));
				break;
			case static_cast<CharType>('%'):
				if (std::distance(i, end) > 2) {
					typename std::char_traits<CharType>::int_type digit;
					CharType first = *(i + 1), second = *(i + 2);
					digit = ((first >= 'A' ? (first & 0xDF) - 'A' + 10 : first - '0') * 16);
					digit += (second >= 'A' ? (second & 0xDF) - 'A' + 10 : second - '0');
					result.push_back(static_cast<CharType>(digit));
					i += 2;
				}
				else {
					result.push_back(static_cast<CharType>('%'));
				}
				break;
			default:
				result.push_back(*i);
				break;
		}
	}
	return result;
}


}} // namespaces

#endif // NEXTWEB_DETAILS_URL_ENCODE_HPP_INCLUDED
