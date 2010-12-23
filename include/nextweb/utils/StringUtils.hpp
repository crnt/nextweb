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

#ifndef NEXTWEB_UTILS_STRING_UTILS_HPP_INCLUDED
#define NEXTWEB_UTILS_STRING_UTILS_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/StaticAssert.hpp"

namespace nextweb { namespace utils {

template <typename Iter, typename Pred> NEXTWEB_INLINE Iter
nextMatched(Iter begin, Iter end, Pred pred) {
	return std::find_if(begin, end, pred);
}

template <typename Iter, typename Pred> NEXTWEB_INLINE Iter
nextNotMatched(Iter begin, Iter end, Pred pred) {
	for (Iter i = begin; i != end; ++i) {
		if (!pred(*i)) return i;
	}
	return end;
}

template <typename Sequence> NEXTWEB_INLINE Sequence
trimLeft(Sequence const &seq) {
	IsSpace<typename Sequence::value_type> checker;
	return Sequence(nextNotMatched(seq.begin(), seq.end(), checker), seq.end());
}

template <typename Sequence> NEXTWEB_INLINE Sequence
trimCharsLeft(Sequence const &seq, typename Sequence::value_type val) {
	IsEqual<typename Sequence::value_type> checker(val);
	return Sequence(seq.begin(), nextNotMatched(seq.rbegin(), seq.rend(), checker).base());
}

template <typename Sequence> NEXTWEB_INLINE Sequence
trimRight(Sequence const &seq) {
	IsSpace<typename Sequence::value_type> checker;
	return Sequence(seq.begin(), nextNotMatched(seq.rbegin(), seq.rend(), checker).base());
}

template <typename Sequence> NEXTWEB_INLINE Sequence
trimCharsRight(Sequence const &seq, typename Sequence::value_type val) {
	IsEqual<typename Sequence::value_type> checker(val);
	return Sequence(seq.begin(), nextNotMatched(seq.rbegin(), seq.rend(), checker).base());
}

template <typename Sequence> NEXTWEB_INLINE Sequence
trim(Sequence const &seq) {
	return trimLeft(trimRight(seq));
}

template <typename Sequence> NEXTWEB_INLINE Sequence
trimChars(Sequence const &seq, typename Sequence::value_type value) {
	return trimCharsLeft(trimCharsRight(seq, value), value);
}

template<typename Sequence> NEXTWEB_INLINE Sequence
truncate(Sequence const &seq, typename Sequence::size_type left, typename Sequence::size_type right) {
	typename Sequence::const_iterator begin = seq.begin(), end = seq.end();
	while (begin != end && left--) {
		++begin;
	}
	while (begin != end && right--) {
		--end;
	}
	return Sequence(begin, end);
}

template <typename Sequence> NEXTWEB_INLINE bool
splitImpl(Sequence const &seq, typename Sequence::const_iterator pos, 
	typename Sequence::size_type size, Sequence &first, Sequence &second) 
{
	typename Sequence::const_iterator begin = seq.begin(), end = seq.end();
	first = Sequence(begin, pos);
	if (pos != end) {
		typename Sequence::const_iterator start = pos;
		std::advance(start, size);
		second = Sequence(start, end);
		return true;
	}
	else {
		second = Sequence(end, end);
		return false;
	}
}

template <typename Sequence, typename Func> NEXTWEB_INLINE bool
splitIfOnce(Sequence const &seq, Func func, Sequence &first, Sequence &second) {
	return splitImpl(seq, std::find_if(seq.begin(), seq.end(), func), 1, first, second);
}

template <typename Sequence> NEXTWEB_INLINE bool
splitOnce(Sequence const &seq, typename Sequence::value_type c, Sequence &first, Sequence &second) {
	return splitImpl(seq, std::find(seq.begin(), seq.end(), c), 1, first, second);
}

template <typename Sequence, typename Delim> NEXTWEB_INLINE bool
splitOnce(Sequence const &seq, Delim const &delim, Sequence &first, Sequence &second, 
	typename Delim::value_type const *sfinae = 0) 
{
	(void) sfinae;
	NEXTWEB_STATIC_ASSERT((IsSame<typename Sequence::value_type, typename Delim::value_type>::RESULT));
	return splitImpl(seq, std::search(seq.begin(), seq.end(), delim.begin(), delim.end()), delim.size(), first, second);
}

template <typename Sequence> NEXTWEB_INLINE bool
splitOnce(Sequence const &seq, typename Sequence::value_type const *chars, Sequence &first, Sequence &second) {
	return splitOnce(seq, makeRange(chars), first, second, static_cast<Range<char const*>::value_type const*>(0));
}

template <typename Sequence, typename Delim> NEXTWEB_INLINE bool
splitFirstOfOnce(Sequence const &seq, Delim const &delim, Sequence &first, Sequence &second, 
	typename Delim::value_type const *sfinae = 0) 
{
	(void) sfinae;
	NEXTWEB_STATIC_ASSERT((IsSame<typename Sequence::value_type, typename Delim::value_type>::RESULT));
	return splitImpl(seq, std::find_first_of(seq.begin(), seq.end(), delim.begin(), delim.end()), 1, first, second);
}

template <typename Sequence> NEXTWEB_INLINE bool
splitFirstOfOnce(Sequence const &seq, typename Sequence::value_type const *chars, Sequence &first, Sequence &second) {
	return splitFirstOfOnce(seq, makeRange(chars), first, second, static_cast<Range<char const*>::value_type const*>(0));
}

template <typename Sequence, typename Prefix> NEXTWEB_INLINE bool
startsWith(Sequence const &seq, Prefix const &prefix, typename Prefix::value_type const *sfinae = 0) {
	(void) sfinae;
	NEXTWEB_STATIC_ASSERT((IsSame<typename Sequence::value_type, typename Prefix::value_type>::RESULT));
	if (static_cast<std::size_t>(prefix.size()) <= static_cast<std::size_t>(seq.size())) {
		return std::equal(prefix.begin(), prefix.end(), seq.begin());
	}
	return false;
}

template <typename Sequence> NEXTWEB_INLINE bool
startsWith(Sequence const &seq, char const *prefix) {
	return startsWith(seq, makeRange(prefix), static_cast<Range<char const*>::value_type const*>(0));
}

template <typename Prefix> NEXTWEB_INLINE bool
startsWith(char const *string, Prefix const &prefix) {
	return startsWith(makeRange(string), prefix, static_cast<typename Prefix::value_type const*>(0));
}

template <typename Sequence, typename Suffix> NEXTWEB_INLINE bool
endsWith(Sequence const &seq, Suffix const &suffix, typename Suffix::value_type const *sfinae = 0) {
	(void) sfinae;
	NEXTWEB_STATIC_ASSERT((IsSame<typename Sequence::value_type, typename Suffix::value_type>::RESULT));
	if (static_cast<std::size_t>(suffix.size()) <= static_cast<std::size_t>(seq.size())) {
		typename Sequence::const_iterator i = seq.begin();
		std::advance(i, seq.size() - suffix.size());
 		return std::equal(suffix.begin(), suffix.end(), i);
	}
	return false;
}

template <typename Sequence> NEXTWEB_INLINE bool
endsWith(Sequence const &seq, char const *suffix) {
	return endsWith(seq, makeRange(suffix), static_cast<Range<char const*>::value_type const*>(0));
}

template <typename Suffix> NEXTWEB_INLINE bool
endsWith(char const *string, Suffix const &suffix) {
	return endsWith(makeRange(string), suffix, static_cast<typename Suffix::value_type const*>(0));
}

}} // namespaces

#endif // NEXTWEB_UTILS_STRING_UTILS_HPP_INCLUDED
