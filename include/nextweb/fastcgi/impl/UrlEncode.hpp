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

#ifndef NEXTWEB_FASTCGI_URL_ENCODE_HPP_INCLUDED
#define NEXTWEB_FASTCGI_URL_ENCODE_HPP_INCLUDED

#include <string>
#include <cstddef>
#include <iterator>

#include "nextweb/Config.hpp"
#include "nextweb/utils/CharTraits.hpp"
#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/StaticAssert.hpp"

namespace nextweb { namespace fastcgi {

template <typename Result, typename Sequence, std::size_t N>
class UrlEncoder;

template <typename Result, typename Sequence>
struct UrlEncoder<Result, Sequence, static_cast<std::size_t>(1)> {
	static Result encode(Sequence const &range);
	static Result decode(Sequence const &range);
};

template <typename Sequence> NEXTWEB_INLINE Sequence
urlencode(Sequence const &range) {
	return UrlEncoder<Sequence, Sequence, sizeof(typename Sequence::value_type)>::encode(range);
}

template <typename Result, typename Sequence> NEXTWEB_INLINE Result
urlencode(Sequence const &range) {
	NEXTWEB_STATIC_ASSERT((utils::IsSame<typename Sequence::value_type, typename Result::value_type>::RESULT));
	return UrlEncoder<Result, Sequence, sizeof(typename Sequence::value_type)>::encode(range);
}

template <typename Sequence> NEXTWEB_INLINE Sequence
urldecode(Sequence const &range) {
	return UrlEncoder<Sequence, Sequence, sizeof(typename Sequence::value_type)>::decode(range);
}

template <typename Result, typename Sequence> NEXTWEB_INLINE Result
urldecode(Sequence const &range) {
	NEXTWEB_STATIC_ASSERT((utils::IsSame<typename Sequence::value_type, typename Result::value_type>::RESULT));
	return UrlEncoder<Result, Sequence, sizeof(typename Sequence::value_type)>::decode(range);
}

template <typename Result, typename Sequence> NEXTWEB_INLINE Result
UrlEncoder<Result, Sequence, static_cast<std::size_t>(1)>::encode(Sequence const &range) {
	
	Result result;
	result.reserve(3 * range.size());
	typedef typename Sequence::value_type Char;
	
	for (typename Sequence::const_iterator i = range.begin(), end = range.end(); i != end; ++i) {
		if (utils::CharTraits<typename utils::RemoveConst<Char>::Type>::isAlphaNumeric(*i)) {
			result.push_back(*i);
			continue;
		}
		switch (*i) {
			case static_cast<Char>('-'): 
			case static_cast<Char>('_'):
			case static_cast<Char>('.'):
			case static_cast<Char>('!'):
			case static_cast<Char>('~'): 
			case static_cast<Char>('*'):
			case static_cast<Char>('('):
			case static_cast<Char>(')'):
			case static_cast<Char>('\''): 
				result.push_back(*i);
				break;
			default:
				result.push_back(static_cast<Char>('%'));
				Char bytes[3] = { 0, 0, 0 };
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

template <typename Result, typename Sequence> NEXTWEB_INLINE Result
UrlEncoder<Result, Sequence, static_cast<std::size_t>(1)>::decode(Sequence const &range) {
	
	Result result;
	result.reserve(range.size());
	typedef typename Sequence::value_type Char;
	
	for (typename Sequence::const_iterator i = range.begin(), end = range.end(); i != end; ++i) {
		switch (*i) {
			case static_cast<Char>('+'): 
				result.push_back(static_cast<Char>(' '));
				break;
			case static_cast<Char>('%'):
				if (std::distance(i, end) > 2) {
					typename Sequence::const_iterator f = i, s = i;
					std::advance(f, 1);
					std::advance(s, 2);
					Char first = *f, second = *s;
					typename std::char_traits<Char>::int_type digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0')) * 16;
					digit += (second >= 'A') ? ((second & 0xDF) - 'A') + 10 : (second - '0');
					result.push_back(static_cast<Char>(digit));
					std::advance(i, 2);
				}
				else {
					result.push_back(static_cast<Char>('%'));
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

#endif // NEXTWEB_FASTCGI_URL_ENCODE_HPP_INCLUDED
