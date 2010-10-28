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

#ifndef NEXTWEB_FASTCGI_ITER_FILE_IMPL_HPP_INCLUDED
#define NEXTWEB_FASTCGI_ITER_FILE_IMPL_HPP_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/StaticAssert.hpp"

#include "nextweb/fastcgi/impl/FileImpl.hpp"
#include "nextweb/fastcgi/impl/StreamBuffer.hpp"

namespace nextweb { namespace fastcgi {

template <typename Iter>
class IterFileImpl : public FileImpl {

public:
	IterFileImpl(std::string const &name, std::string const &type, utils::Range<Iter> const &cont);
	virtual ~IterFileImpl();

	virtual std::streamsize size() const;
	virtual std::string const& name() const;
	virtual std::string const& contentType() const;

	virtual std::istream& stream();
	virtual void save(std::string const &name) const;

private:
	IterFileImpl(IterFileImpl const &);
	IterFileImpl& operator = (IterFileImpl const &);

private:
	std::string name_, type_;
	StreamBuffer<Iter> buffer_;
	utils::Range<Iter> content_;	
	std::istream stream_;
};

template <typename Iter> NEXTWEB_INLINE
IterFileImpl<Iter>::IterFileImpl(std::string const &name, std::string const &type, utils::Range<Iter> const &cont) :
	name_(name), type_(type), buffer_(cont.begin(), cont.end()), content_(cont), stream_(&buffer_)
{
}

template <typename Iter> NEXTWEB_INLINE 
IterFileImpl<Iter>::~IterFileImpl() {
}

template <typename Iter> NEXTWEB_INLINE std::streamsize
IterFileImpl<Iter>::size() const {
	NEXTWEB_STATIC_ASSERT(sizeof(std::streamsize) >= sizeof(typename utils::Range<Iter>::size_type));
	return static_cast<std::streamsize>(content_.size());
}

template <typename Iter> NEXTWEB_INLINE std::string const&
IterFileImpl<Iter>::name() const {
	return name_;
}

template <typename Iter> NEXTWEB_INLINE std::string const&
IterFileImpl<Iter>::contentType() const {
	return type_;
}

template <typename Iter> NEXTWEB_INLINE std::istream&
IterFileImpl<Iter>::stream() {
	return stream_;
}

template <typename Iter> NEXTWEB_INLINE void
IterFileImpl<Iter>::save(std::string const &name) const {
	std::ofstream file(name);
	file.exceptions(std::ios::badbit | std::ios::failbit);
	std::copy(content_.begin(), content_.end(), std::ostream_iterator<typename std::iterator_traits<Iter>::value_type>(file));
}

template <> NEXTWEB_INLINE void
IterFileImpl<char const*>::save(std::string const &name) const {
	// implement bufferless write to disk
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_ITER_FILE_IMPL_HPP_INCLUDED
