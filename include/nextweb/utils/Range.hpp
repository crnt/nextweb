// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef NEXTWEB_UTILS_RANGE_HPP_INCLUDED
#define NEXTWEB_UTILS_RANGE_HPP_INCLUDED

#include <cstring>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <functional>

#include "nextweb/Config.hpp"
#include "nextweb/utils/Integer.hpp"

namespace nextweb { namespace utils {

template <typename Iter, typename Tag>
class RangeBase;

template <typename Iter>
class RangeBase<Iter, std::forward_iterator_tag> {

public:
	RangeBase();
	RangeBase(Iter begin, Iter end);

	typedef Iter iterator;
	typedef Iter const_iterator;
	typedef typename std::iterator_traits<Iter>::value_type value_type;
	typedef typename MakeUnsigned<typename std::iterator_traits<Iter>::difference_type>::Type size_type;

	iterator end();
	const_iterator end() const;

	iterator begin();
	const_iterator begin() const;

	void clear();
	bool empty() const;
	size_type size() const;

private:
	Iter begin_, end_;
};

template <typename Iter>
class RangeBase<Iter, std::bidirectional_iterator_tag> : public RangeBase<Iter, std::forward_iterator_tag> {

public:
	RangeBase();
	RangeBase(Iter begin, Iter end);
	
	typedef std::reverse_iterator<Iter> reverse_iterator;
	typedef std::reverse_iterator<Iter> const_reverse_iterator;

	reverse_iterator rend();
	const_reverse_iterator rend() const;

	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
};

template <typename Iter>
class RangeBase<Iter, std::random_access_iterator_tag> : public RangeBase<Iter, std::bidirectional_iterator_tag> {

public:
	RangeBase();
	RangeBase(Iter begin, Iter end);
	
	typedef typename std::iterator_traits<Iter>::value_type value_type;
	typedef typename MakeUnsigned<typename std::iterator_traits<Iter>::difference_type>::Type size_type;

	value_type const& operator [] (size_type index) const;
};

template <typename Iter>
class Range : public RangeBase<Iter, typename std::iterator_traits<Iter>::iterator_category> {

public:
	Range();
	Range(Iter begin, Iter end);
};

template <typename Iter> NEXTWEB_INLINE
RangeBase<Iter, std::forward_iterator_tag>::RangeBase() :
	begin_(), end_(begin_)
{
}

template <typename Iter> NEXTWEB_INLINE
RangeBase<Iter, std::forward_iterator_tag>::RangeBase(Iter begin, Iter end) :
	begin_(begin), end_(end)
{
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::iterator 
RangeBase<Iter, std::forward_iterator_tag>::end() {
	return end_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::const_iterator
RangeBase<Iter, std::forward_iterator_tag>::end() const {
	return end_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::iterator 
RangeBase<Iter, std::forward_iterator_tag>::begin() {
	return begin_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::const_iterator
RangeBase<Iter, std::forward_iterator_tag>::begin() const {
	return begin_;
}

template <typename Iter> NEXTWEB_INLINE void
RangeBase<Iter, std::forward_iterator_tag>::clear() {
	begin_ = end_;
}

template <typename Iter> NEXTWEB_INLINE bool
RangeBase<Iter, std::forward_iterator_tag>::empty() const {
	return begin_ == end_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::size_type
RangeBase<Iter, std::forward_iterator_tag>::size() const {
	return end_ == begin_ ? 0 : std::distance(begin_, end_);
}

template <typename Iter> NEXTWEB_INLINE 
RangeBase<Iter, std::bidirectional_iterator_tag>::RangeBase() :
	RangeBase<Iter, std::forward_iterator_tag>()
{
}

template <typename Iter> NEXTWEB_INLINE 
RangeBase<Iter, std::bidirectional_iterator_tag>::RangeBase(Iter begin, Iter end) :
	RangeBase<Iter, std::forward_iterator_tag>(begin, end)
{
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rend() {
	return reverse_iterator(this->begin());
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::const_reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rend() const {
	return const_reverse_iterator(this->begin());
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rbegin() {
	return reverse_iterator(this->end());
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::const_reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rbegin() const {
	return const_reverse_iterator(this->end());
}

template <typename Iter> NEXTWEB_INLINE 
RangeBase<Iter, std::random_access_iterator_tag>::RangeBase() :
	RangeBase<Iter, std::bidirectional_iterator_tag>()
{
}

template <typename Iter> NEXTWEB_INLINE 
RangeBase<Iter, std::random_access_iterator_tag>::RangeBase(Iter begin, Iter end) :
	RangeBase<Iter, std::bidirectional_iterator_tag>(begin, end)
{
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::random_access_iterator_tag>::value_type const&
RangeBase<Iter, std::random_access_iterator_tag>::operator [] (typename RangeBase<Iter, typename std::random_access_iterator_tag>::size_type index) const {
 	return *(this->begin() + index);
}

template <typename Iter> NEXTWEB_INLINE
Range<Iter>::Range() :
	RangeBase<Iter, typename std::iterator_traits<Iter>::iterator_category>()
{
}

template <typename Iter> NEXTWEB_INLINE
Range<Iter>::Range(Iter begin, Iter end) :
	RangeBase<Iter, typename std::iterator_traits<Iter>::iterator_category>(begin, end)
{
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator < (Range<Iter> const &range, Range<Other> const &other) {
	return std::lexicographical_compare(range.begin(), range.end(), other.begin(), other.end());
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator > (Range<Iter> const &range, Range<Other> const &other) {
	std::greater<typename std::iterator_traits<Iter>::value_type> pred;
	return std::lexicographical_compare(range.begin(), range.end(), other.begin(), other.end(), pred);
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator == (Range<Iter> const &range, Range<Other> const &other) {
	if (range.size() == other.size()) {
		return std::equal(range.begin(), range.end(), other.begin());
	}
	return false;
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator != (Range<Iter> const &range, Range<Other> const &other) {
	return !operator == (range, other);
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator <= (Range<Iter> const &range, Range<Other> const &other) {
	return !operator > (range, other);
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator >= (Range<Iter> const &range, Range<Other> const &other) {
	return !operator < (range, other);
}

template <typename Iter, typename Char, typename Traits> NEXTWEB_INLINE std::basic_ostream<Char, Traits>&
operator << (std::basic_ostream<Char, Traits> &stream, Range<Iter> const &range) {
	std::copy(range.begin(), range.end(), std::ostream_iterator<typename Range<Iter>::value_type, Char, Traits>(stream));
	return stream;
}

template <typename Cont> NEXTWEB_INLINE Range<typename Cont::const_iterator> 
makeRange(Cont const &cont) {
	return Range<typename Cont::const_iterator>(cont.begin(), cont.end());
}

template <typename Iter> NEXTWEB_INLINE Range<Iter>
makeRange(Iter begin, Iter end) {
	return Range<Iter>(begin, end);
}

Range<char const*>
makeRange(char const *str);

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/Range.hpp"
#endif

#endif // NEXTWEB_UTILS_RANGE_HPP_INCLUDED
