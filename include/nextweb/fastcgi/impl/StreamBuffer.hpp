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

#ifndef NEXTWEB_FASTCGI_STREAM_BUFFER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_STREAM_BUFFER_HPP_INCLUDED

#include <iostream>
#include <iterator>

#include "nextweb/Config.hpp"

namespace nextweb { namespace fastcgi {

template <typename Iter, typename Traits = std::char_traits<typename std::iterator_traits<Iter>::value_type> >
class StreamBuffer : public std::basic_streambuf<typename std::iterator_traits<Iter>::value_type, Traits> {

public:
	StreamBuffer(Iter begin, Iter end);
	virtual ~StreamBuffer();

	typedef typename std::iterator_traits<Iter>::value_type CharType;
	typedef std::basic_streambuf<CharType> BaseType;

private:
	StreamBuffer(StreamBuffer const &);
	StreamBuffer& operator = (StreamBuffer const &);

private:
	Iter current_;
	Iter const begin_, end_;
};

template <typename Iter, typename Traits> NEXTWEB_INLINE
StreamBuffer<Iter, Traits>::StreamBuffer(Iter begin, Iter end) :
	current_(begin), begin_(begin), end_(end)
{
}

template <typename Iter, typename Traits> NEXTWEB_INLINE
StreamBuffer<Iter, Traits>::~StreamBuffer() {
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_STREAM_BUFFER_HPP_INCLUDED
