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

#ifndef NEXTWEB_FASTCGI_LINE_READER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_LINE_READER_HPP_INCLUDED

#include <iterator>
#include <algorithm>

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/Iterator.hpp"
#include "nextweb/utils/StringUtils.hpp"

namespace nextweb { namespace fastcgi {

template <typename Sequence>
class LineReader : public Enumeration<Sequence> {

public:
	LineReader(Sequence const &seq);
	virtual ~LineReader();
	
	bool wasMultiline() const;
	bool hasMoreElements() const;
	Sequence nextElement() const;

private:
	LineReader(LineReader const &);
	LineReader& operator = (LineReader const &);

	typedef typename Sequence::iterator IterType;
	typedef typename Sequence::value_type CharType;

private:
	bool multiline_;
	mutable Sequence source_;
	utils::IsLineEnd<CharType> lineEndChecker_;
};

template <typename Iter>
class LineEndFilter : public utils::Iterator<LineEndFilter<Iter>, std::bidirectional_iterator_tag, typename std::iterator_traits<Iter>::value_type> {

public:
	LineEndFilter();
	LineEndFilter(Iter begin, Iter end);

	typedef typename std::iterator_traits<Iter>::value_type CharType;
	typedef typename std::iterator_traits<Iter>::reference ReferenceType;

	void increment();
	void decrement();
	bool equal(LineEndFilter<Iter> const &other) const;
	ReferenceType dereference();

private:
	Iter i_;
	Iter const begin_, end_;
	utils::IsSpace<CharType> spaceChecker_;
	utils::IsLineEnd<CharType> lineEndChecker_;
};

template <typename Sequence> NEXTWEB_INLINE
LineReader<Sequence>::LineReader(Sequence const &seq) :
	multiline_(false), source_(seq), lineEndChecker_()
{
}

template <typename Sequence> NEXTWEB_INLINE
LineReader<Sequence>::~LineReader() {
}

template <typename Sequence> NEXTWEB_INLINE bool
LineReader<Sequence>::wasMultiline() const {
	return multiline_;
}

template <typename Sequence> NEXTWEB_INLINE bool
LineReader<Sequence>::hasMoreElements() const {
	return !source_.empty();
}

template <typename Sequence> NEXTWEB_INLINE Sequence
LineReader<Sequence>::nextElement() const {
	
	Sequence res;
	IterType begin = source_.begin(), end = source_.end(), lineEnd, newLine = begin;
	while (true) {
		lineEnd = utils::nextMatched(newLine, end, lineEndChecker_);
		newLine = utils::nextNotMatched(lineEnd, end, lineEndChecker_);
		if (newLine == end) {
			res = Sequence(begin, lineEnd);
			source_.clear();
			break;
		}
		else if (!utils::IsSpace<CharType>::check(*newLine)) {
			res = Sequence(begin, lineEnd);
			source_ = Sequence(newLine, end);
			break;
		}
	}
	return res;
}

template <typename Iter> NEXTWEB_INLINE 
LineEndFilter<Iter>::LineEndFilter() :
	i_(), begin_(), end_()
{
}

template <typename Iter> NEXTWEB_INLINE 
LineEndFilter<Iter>::LineEndFilter(Iter begin, Iter end) :
	i_(begin), begin_(), end_(end)
{
}

template <typename Iter> NEXTWEB_INLINE void
LineEndFilter<Iter>::increment() {
}

template <typename Iter> NEXTWEB_INLINE void
LineEndFilter<Iter>::decrement() {
}

template <typename Iter> NEXTWEB_INLINE bool
LineEndFilter<Iter>::equal(LineEndFilter<Iter> const &other) const {
	return (i_ == other.i_) && (end_ == other.end_);
}

template <typename Iter> NEXTWEB_INLINE typename LineEndFilter<Iter>::ReferenceType
LineEndFilter<Iter>::dereference() {
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_LINE_READER_HPP_INCLUDED
