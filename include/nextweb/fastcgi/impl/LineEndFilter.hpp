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

#ifndef NEXTWEB_FASTCGI_LINE_END_FILTER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_LINE_END_FILTER_HPP_INCLUDED

#include <cassert>
#include <iterator>

#include "nextweb/Config.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/Iterator.hpp"
#include "nextweb/utils/StringUtils.hpp"

namespace nextweb { namespace fastcgi {

template <typename Iter>
class LineEndFilter : public utils::Iterator<LineEndFilter<Iter>, std::bidirectional_iterator_tag, typename std::iterator_traits<Iter>::value_type> {

public:
	LineEndFilter();
	LineEndFilter(Iter begin, Iter end);

	typedef LineEndFilter<Iter> Type;
	typedef typename std::iterator_traits<Iter>::reference ReferenceType;

	void increment();
	void decrement();
	bool equal(LineEndFilter<Iter> const &other) const;
	
	ReferenceType dereference();

	Iter base() const;

private:
	void seekForward();
	void seekReverse();
	
	typedef typename std::iterator_traits<Iter>::value_type CharType;
	static CharType SPACE;

private:
	Iter i_, begin_, end_;
	bool empty_, filterTriggered_;
	utils::IsSpace<CharType> spaceChecker_;
	utils::IsLineEnd<CharType> lineEndChecker_;
};

template <typename Iter> typename LineEndFilter<Iter>::CharType 
LineEndFilter<Iter>::SPACE = static_cast<typename std::iterator_traits<Iter>::value_type>(' ');

template <typename Iter> NEXTWEB_INLINE 
LineEndFilter<Iter>::LineEndFilter() :
	i_(), begin_(), end_(), empty_(true), filterTriggered_(false)
{
}

template <typename Iter> NEXTWEB_INLINE 
LineEndFilter<Iter>::LineEndFilter(Iter begin, Iter end) :
	i_(begin), begin_(begin), end_(end), empty_(false), filterTriggered_(false)
{
	seekForward();
}

template <typename Iter> NEXTWEB_INLINE void
LineEndFilter<Iter>::increment() {
	assert(!empty_);
	if (filterTriggered_) {
		filterTriggered_ = false;
	}
	else {
		++i_;
		seekForward();
	}
}

template <typename Iter> NEXTWEB_INLINE void
LineEndFilter<Iter>::decrement() {
	assert(!empty_);
	if (filterTriggered_) {
		filterTriggered_ = false;
	}
	else {
		--i_;
		seekReverse();
	}
}

template <typename Iter> NEXTWEB_INLINE bool
LineEndFilter<Iter>::equal(LineEndFilter<Iter> const &other) const {
	return (!empty_ && !other.empty_) ? i_ == other.i_ : (empty_ && other.empty_);
}

template <typename Iter> NEXTWEB_INLINE typename LineEndFilter<Iter>::ReferenceType
LineEndFilter<Iter>::dereference() {
	assert(!empty_);
	return (filterTriggered_) ? SPACE : *i_;
}

template <typename Iter> NEXTWEB_INLINE Iter
LineEndFilter<Iter>::base() const {
	return i_;
}

template <typename Iter> NEXTWEB_INLINE void
LineEndFilter<Iter>::seekForward() {
	if ((end_ != i_) && lineEndChecker_(*i_)) {
		Iter newLine = utils::nextNotMatched(i_, end_, lineEndChecker_);
		if (end_ != newLine && spaceChecker_(*newLine)) {
			i_ = utils::nextNotMatched(newLine, end_, spaceChecker_);
			filterTriggered_ = true;
		}
	}
}

template <typename Iter> NEXTWEB_INLINE void
LineEndFilter<Iter>::seekReverse() {
	typedef std::reverse_iterator<Iter> IteratorType;
	if ((end_ != i_) && spaceChecker_(*i_)) {
		IteratorType rend(begin_);
		IteratorType tokenEnd = utils::nextNotMatched(IteratorType(i_), rend, spaceChecker_);
		if (lineEndChecker_(*tokenEnd)) {
			i_ = --(nextNotMatched(tokenEnd, rend, lineEndChecker_).base());
			filterTriggered_ = true;
		}
	}
}

template <typename Iter> NEXTWEB_INLINE utils::Range<typename LineEndFilter<Iter>::Type>
makeLineEndFiltered(Iter begin, Iter end) {
	typedef LineEndFilter<Iter> IteratorType;
	IteratorType filteredBegin(begin, end), filteredEnd(end, end);
	return utils::makeRange(filteredBegin, filteredEnd);
}

template <typename Iter> NEXTWEB_INLINE utils::Range<Iter>
makeLineEndUnfiltered(LineEndFilter<Iter> begin, LineEndFilter<Iter> end) {
	return utils::makeRange(begin.base(), end.base());
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_LINE_END_FILTER_HPP_INCLUDED
