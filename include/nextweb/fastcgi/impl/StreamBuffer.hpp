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

#ifndef NEXTWEB_FASTCGI_STREAM_BUFFER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_STREAM_BUFFER_HPP_INCLUDED

#include <limits>
#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "nextweb/Config.hpp"

namespace nextweb { namespace fastcgi {

template <typename Iter, typename Traits = std::char_traits<typename std::iterator_traits<Iter>::value_type> >
class StreamBuffer : public std::basic_streambuf<typename std::iterator_traits<Iter>::value_type, Traits> {

public:
	StreamBuffer(Iter begin, Iter end);
	virtual ~StreamBuffer();

	typedef typename std::iterator_traits<Iter>::value_type CharType;
	typedef typename std::iterator_traits<Iter>::difference_type DistanceType;
	typedef std::basic_streambuf<CharType, Traits> BaseType;
	
	typedef typename BaseType::int_type IntType;
	typedef typename BaseType::pos_type PosType;
	typedef typename BaseType::off_type OffType;
	
	virtual IntType underflow();
	virtual IntType pbackfail(IntType value);

	virtual PosType seekpos(PosType pos, std::ios::openmode which);
	virtual PosType seekoff(OffType off, std::ios::seekdir dir, std::ios::openmode which);

	virtual std::streamsize showmanyc();

private:
	StreamBuffer(StreamBuffer const &);
	StreamBuffer& operator = (StreamBuffer const &);

	IntType fill();
	using BaseType::gptr;
	using BaseType::setg;
	using BaseType::eback;
	using BaseType::egptr;
	
	static PosType const BAD_POSITION;
	static std::size_t const BUFFER_SIZE = 4096;

	NEXTWEB_STATIC_ASSERT(std::numeric_limits<OffType>::is_signed);
	NEXTWEB_STATIC_ASSERT(std::numeric_limits<DistanceType>::is_signed);
		
private:
	Iter current_;
	Iter const begin_, end_;
	CharType buffer_[BUFFER_SIZE];
};

template <typename Iter, typename Traits> typename StreamBuffer<Iter, Traits>::PosType const
StreamBuffer<Iter, Traits>::BAD_POSITION(static_cast<std::streamoff>(-1));

template <typename Iter, typename Traits> std::size_t const
StreamBuffer<Iter, Traits>::BUFFER_SIZE;

template <typename Iter, typename Traits> NEXTWEB_INLINE
StreamBuffer<Iter, Traits>::StreamBuffer(Iter begin, Iter end) :
	BaseType(), current_(begin), begin_(begin), end_(end)
{
	setg(buffer_, buffer_, buffer_);
}

template <typename Iter, typename Traits> NEXTWEB_INLINE
StreamBuffer<Iter, Traits>::~StreamBuffer() {
}

template <typename Iter, typename Traits> NEXTWEB_INLINE typename StreamBuffer<Iter, Traits>::IntType
StreamBuffer<Iter, Traits>::underflow() {
	assert(gptr() == egptr());
	if (end_ == current_) {
		return Traits::eof();
	}
	return fill();
}

template <typename Iter, typename Traits> NEXTWEB_INLINE typename StreamBuffer<Iter, Traits>::IntType
StreamBuffer<Iter, Traits>::pbackfail(typename StreamBuffer<Iter, Traits>::IntType value) {
	assert(gptr() == eback());
	if (begin_ == current_) {
		return Traits::eof();
	}
	--current_;
	DistanceType dist = static_cast<DistanceType>(egptr() - eback());
	memmove(buffer_ + 1, buffer_, dist - 1);
	buffer_[0] = value;
	return Traits::to_int_type(value);
}

template <typename Iter, typename Traits> NEXTWEB_INLINE typename StreamBuffer<Iter, Traits>::PosType
StreamBuffer<Iter, Traits>::seekpos(typename StreamBuffer<Iter, Traits>::PosType pos, std::ios::openmode which) {
	return seekoff(static_cast<OffType>(pos), std::ios::beg, which);
}

template <typename Iter, typename Traits> NEXTWEB_INLINE typename StreamBuffer<Iter, Traits>::PosType
StreamBuffer<Iter, Traits>::seekoff(typename StreamBuffer<Iter, Traits>::OffType off, std::ios::seekdir dir, std::ios::openmode which) {
	
	if (!(which & std::ios::in)) {
		return BAD_POSITION;
	}		
	
	DistanceType size = std::distance(begin_, end_);
	DistanceType dist = static_cast<DistanceType>(off);
	DistanceType position = static_cast<DistanceType>(-1);
	
	if ((std::ios::beg == dir) && (dist >= 0) && (dist < size)) {
		position = dist;
	}
	else if ((std::ios::end == dir) && (dist >= 0) && (dist < size)) {
		position = size - dist;
	}
	else if (std::ios::cur == dir) {
		position = std::distance(begin_, current_) - static_cast<DistanceType>(egptr() - gptr()) + dist;
		if ((position < 0) || (position >= size)) {
			return BAD_POSITION;
		}
	}
	if (static_cast<DistanceType>(-1) != position) {
		current_ = begin_;
		std::advance(current_, position);
		fill();
		return static_cast<PosType>(position);
	}
	return BAD_POSITION;
}

template <typename Iter, typename Traits> NEXTWEB_INLINE typename StreamBuffer<Iter, Traits>::IntType
StreamBuffer<Iter, Traits>::fill() {
	std::size_t size = 0;
	for (; end_ != current_ && size < BUFFER_SIZE; ++current_, ++size) {
		buffer_[size] = *current_;
	}
	setg(buffer_, buffer_, buffer_ + size);
	return Traits::to_int_type(buffer_[0]);
}

template <typename Iter, typename Traits> NEXTWEB_INLINE std::streamsize
StreamBuffer<Iter, Traits>::showmanyc() {
	assert(gptr() == egptr());
	return static_cast<std::streamsize>(std::distance(current_, end_));
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_STREAM_BUFFER_HPP_INCLUDED
