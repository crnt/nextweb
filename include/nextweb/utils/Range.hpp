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

#ifndef NEXTWEB_UTILS_RANGE_HPP_INCLUDED
#define NEXTWEB_UTILS_RANGE_HPP_INCLUDED

#include <cassert>
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
	
	typedef typename std::iterator_traits<Iter>::pointer pointer;
	typedef typename std::iterator_traits<Iter>::value_type value_type;

	typedef typename std::iterator_traits<Iter>::reference reference;
	typedef typename std::iterator_traits<Iter>::reference const const_reference;

	typedef typename std::iterator_traits<Iter>::difference_type difference_type;
	typedef typename MakeUnsigned<typename std::iterator_traits<Iter>::difference_type>::Type size_type;

	iterator end();
	const_iterator end() const;

	iterator begin();
	const_iterator begin() const;

	void clear();
	bool empty() const;
	size_type size() const;

private:
	bool empty_;
	Iter begin_, end_;
};

template <typename Iter>
class RangeBase<Iter, std::bidirectional_iterator_tag> : public RangeBase<Iter, std::forward_iterator_tag> {

public:
	RangeBase();
	RangeBase(Iter begin, Iter end);

	typedef RangeBase<Iter, std::forward_iterator_tag> BaseType;

	typedef typename BaseType::iterator iterator;
	typedef typename BaseType::const_iterator const_iterator;
	
	typedef typename BaseType::pointer pointer;
	typedef typename BaseType::value_type value_type;

	typedef typename BaseType::reference reference;
	typedef typename BaseType::reference const const_reference;

	typedef typename BaseType::difference_type difference_type;
	typedef typename BaseType::size_type size_type;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	using BaseType::end;
	using BaseType::begin;
	
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
	
	typedef RangeBase<Iter, std::bidirectional_iterator_tag> BaseType;

	typedef typename BaseType::iterator iterator;
	typedef typename BaseType::const_iterator const_iterator;
	
	typedef typename BaseType::pointer pointer;
	typedef typename BaseType::value_type value_type;

	typedef typename BaseType::reference reference;
	typedef typename BaseType::reference const const_reference;

	typedef typename BaseType::difference_type difference_type;
	typedef typename BaseType::size_type size_type;

	typedef typename BaseType::reverse_iterator reverse_iterator;
	typedef typename BaseType::const_reverse_iterator const_reverse_iterator;

	const_reference operator [] (size_type index) const;
};

template <typename Iter>
class Range : public RangeBase<Iter, typename std::iterator_traits<Iter>::iterator_category> {

public:
	Range();
	Range(Iter begin, Iter end);
};

template <typename Iter> NEXTWEB_INLINE
RangeBase<Iter, std::forward_iterator_tag>::RangeBase() :
	empty_(true), begin_(), end_()
{
}

template <typename Iter> NEXTWEB_INLINE
RangeBase<Iter, std::forward_iterator_tag>::RangeBase(Iter begin, Iter end) :
	empty_(false), begin_(begin), end_(end)
{
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::iterator 
RangeBase<Iter, std::forward_iterator_tag>::end() {
	assert(!empty_);
	return end_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::const_iterator
RangeBase<Iter, std::forward_iterator_tag>::end() const {
	assert(!empty_);
	return end_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::iterator 
RangeBase<Iter, std::forward_iterator_tag>::begin() {
	assert(!empty_);
	return begin_;
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::const_iterator
RangeBase<Iter, std::forward_iterator_tag>::begin() const {
	assert(!empty_);
	return begin_;
}

template <typename Iter> NEXTWEB_INLINE void
RangeBase<Iter, std::forward_iterator_tag>::clear() {
	begin_ = end_;
	empty_ = true;
}

template <typename Iter> NEXTWEB_INLINE bool
RangeBase<Iter, std::forward_iterator_tag>::empty() const {
	return empty_ ? true : (begin_ == end_);
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::forward_iterator_tag>::size_type
RangeBase<Iter, std::forward_iterator_tag>::size() const {
	return empty() ? 0 : std::distance(begin_, end_);
}

template <typename Iter> NEXTWEB_INLINE 
RangeBase<Iter, std::bidirectional_iterator_tag>::RangeBase() :
	BaseType()
{
}

template <typename Iter> NEXTWEB_INLINE 
RangeBase<Iter, std::bidirectional_iterator_tag>::RangeBase(Iter begin, Iter end) :
	BaseType(begin, end)
{
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rend() {
	return reverse_iterator(begin());
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::const_reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rend() const {
	return const_reverse_iterator(begin());
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rbegin() {
	return reverse_iterator(end());
}

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::bidirectional_iterator_tag>::const_reverse_iterator
RangeBase<Iter, std::bidirectional_iterator_tag>::rbegin() const {
	return const_reverse_iterator(end());
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

template <typename Iter> NEXTWEB_INLINE typename RangeBase<Iter, std::random_access_iterator_tag>::const_reference
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
	return (!range.empty() && !other.empty()) ? std::lexicographical_compare(range.begin(), range.end(), other.begin(), other.end()) : !other.empty();
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator > (Range<Iter> const &range, Range<Other> const &other) {
	std::greater<typename std::iterator_traits<Iter>::value_type> pred;
	return (!range.empty() && !other.empty()) ? std::lexicographical_compare(range.begin(), range.end(), other.begin(), other.end(), pred) : !range.empty();
}

template <typename Iter, typename Other> NEXTWEB_INLINE bool
operator == (Range<Iter> const &range, Range<Other> const &other) {
	if (range.size() == other.size()) {
		return range.empty() ? true : std::equal(range.begin(), range.end(), other.begin());
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
	if (!range.empty()) {
		std::copy(range.begin(), range.end(), std::ostream_iterator<typename Range<Iter>::value_type, Char, Traits>(stream));
	}
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

Range<char*>
makeRange(char *str);

Range<char const*>
makeRange(char const *str);

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/Range.hpp"
#endif

#endif // NEXTWEB_UTILS_RANGE_HPP_INCLUDED
