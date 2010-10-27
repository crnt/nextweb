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
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/StringUtils.hpp"

namespace nextweb { namespace fastcgi {

template <typename Iter>
class LineReader {

public:
	LineReader(Iter begin, Iter end);
	bool isMultiline() const;
	bool readLine(utils::Range<Iter> &line);

private:
	LineReader(LineReader const &);
	LineReader& operator = (LineReader const &);
	typedef typename std::iterator_traits<Iter>::value_type CharType;

private:
	Iter begin_;
	Iter const end_;
	bool multiline_;
	utils::IsSpace<CharType> spaceCheck_;
	utils::IsLineEnd<CharType> lineEndCheck_;
};

template <typename Iter> NEXTWEB_INLINE
LineReader<Iter>::LineReader(Iter begin, Iter end) :
	begin_(begin), end_(end), multiline_(false)
{
}


template <typename Iter> NEXTWEB_INLINE bool
LineReader<Iter>::isMultiline() const {
	return multiline_;
}

template <typename Iter> NEXTWEB_INLINE bool
LineReader<Iter>::readLine(utils::Range<Iter> &line) {
	
	if (end_ == begin_) return false;

	multiline_ = false;
	Iter position = begin_;

	while (true) {

		Iter lineEnd = utils::nextMatched(position, end_, lineEndCheck_);
		Iter newLine = utils::nextNotMatched(lineEnd, end_, lineEndCheck_);
		
		line = utils::makeRange(begin_, lineEnd);
		if (end_ == lineEnd || end_ == newLine) {
			begin_ = end_;
			return true;
		}
		if (!spaceCheck_(*newLine)) {
			begin_ = newLine;
			return true;
		}
		multiline_ = true;
		position = newLine;
	}
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_LINE_READER_HPP_INCLUDED
