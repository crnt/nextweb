/** @file Range.hpp */
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

#ifndef NEXTWEB_DETAILS_RANGE_HPP_INCLUDED
#define NEXTWEB_DETAILS_RANGE_HPP_INCLUDED

#include <cstring>
#include <iostream>
#include <iterator>
#include <functional>

namespace nextweb { namespace details {

template <typename Iter>
class Range {

public:
	Range();
	Range(Iter begin, Iter end);

	typedef Iter iterator;
	typedef Iter const_iterator;
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	typedef typename std::iterator_traits<Iter>::value_type value_type;
	typedef typename std::iterator_traits<Iter>::difference_type size_type;
	
	iterator end();
	const_iterator end() const;

	iterator begin();
	const_iterator begin() const;

	reverse_iterator rend();
	const_reverse_iterator rend() const;
	
	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
	
	bool empty() const;
	size_type size() const;

private:
	Iter begin_, end_;
};

template <typename Iter> inline
Range<Iter>::Range() :
	begin_(), end_()
{
}

template <typename Iter> inline
Range<Iter>::Range(Iter begin, Iter end) :
	begin_(begin), end_(end)
{
}

template <typename Iter> inline typename Range<Iter>::iterator 
Range<Iter>::end() {
	return end_;
}

template <typename Iter> inline typename Range<Iter>::const_iterator 
Range<Iter>::end() const {
	return end_;
}

template <typename Iter> inline typename Range<Iter>::iterator 
Range<Iter>::begin() {
	return begin_;
}

template <typename Iter> inline typename Range<Iter>::const_iterator 
Range<Iter>::begin() const {
	return begin_;
}
	
template <typename Iter> inline typename Range<Iter>::reverse_iterator
Range<Iter>::rend() {
	return reverse_iterator(begin_);
}

template <typename Iter> inline typename Range<Iter>::const_reverse_iterator
Range<Iter>::rend() const {
	return const_reverse_iterator(begin_);
}
	
template <typename Iter> inline typename Range<Iter>::reverse_iterator
Range<Iter>::rbegin() {
	return const_reverse_iterator(end_);
}

template <typename Iter> inline typename Range<Iter>::const_reverse_iterator
Range<Iter>::rbegin() const {
	return const_reverse_iterator(end_);
}

template <typename Iter> inline bool
Range<Iter>::empty() const {
	return begin_ == end_;
}

template <typename Iter> inline typename Range<Iter>::size_type
Range<Iter>::size() const {
	return std::distance(begin_, end_);
}

template <typename Iter> std::ostream&
operator << (std::ostream &stream, Range<Iter> const &range) {
	std::copy(range.begin(), range.end(), 
		std::ostream_iterator<typename std::iterator_traits<Iter>::value_type>(stream));
	return stream;
}

template <typename Iter, typename Other> inline bool
operator < (Range<Iter> const &range, Range<Other> const &other) {
	return std::lexicographical_compare(range.begin(), range.end(), other.begin(), other.end());
}

template <typename Iter, typename Other> inline bool
operator > (Range<Iter> const &range, Range<Other> const &other) {
	std::greater<typename std::iterator_traits<Iter>::value_type> pred;
	return std::lexicographical_compare(range.begin(), range.end(), other.begin(), other.end(), pred);
}

template <typename Iter, typename Other> inline bool
operator == (Range<Iter> const &range, Range<Other> const &other) {
	if (range.size() == other.size()) {
		return std::equal(range.begin(), range.end(), other.begin());
	}
	return false;
}

template <typename Iter, typename Other> inline bool
operator != (Range<Iter> const &range, Range<Other> const &other) {
	return !operator == (range, other);
}

template <typename Iter, typename Other> inline bool
operator <= (Range<Iter> const &range, Range<Other> const &other) {
	return !operator > (range, other);
}

template <typename Iter, typename Other> inline bool
operator >= (Range<Iter> const &range, Range<Other> const &other) {
	return !operator < (range, other);
}

inline Range<char const*>
makeRange(char const *str) {
	return Range<char const*>(str, str + strlen(str));
}

template <typename Cont> inline Range<typename Cont::const_iterator> 
makeRange(Cont const &cont) {
	return Range<typename Cont::const_iterator>(cont.begin(), cont.end());
}

template <typename Iter> inline Range<Iter>
makeRange(Iter begin, Iter end) {
	return Range<Iter>(begin, end);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_RANGE_HPP_INCLUDED
