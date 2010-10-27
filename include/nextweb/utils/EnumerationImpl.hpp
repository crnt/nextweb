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

#ifndef NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED
#define NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"

namespace nextweb { namespace utils {

template <typename Iter>
class EnumerationImpl : public Enumeration<typename std::iterator_traits<Iter>::value_type> {

public:
	EnumerationImpl(Iter begin, Iter end);
	virtual ~EnumerationImpl();
	
	virtual bool hasMoreElements() const;
	virtual typename std::iterator_traits<Iter>::value_type nextElement() const;

private:
	EnumerationImpl(EnumerationImpl const &);
	EnumerationImpl& operator = (EnumerationImpl const &);

private:
	const Iter end_;
	mutable Iter begin_;
};

template <typename Iter> NEXTWEB_INLINE
EnumerationImpl<Iter>::EnumerationImpl(Iter begin, Iter end) :
	end_(end), begin_(begin)
{
}

template <typename Iter> NEXTWEB_INLINE 
EnumerationImpl<Iter>::~EnumerationImpl() {
}

template <typename Iter> NEXTWEB_INLINE bool
EnumerationImpl<Iter>::hasMoreElements() const {
	return begin_ != end_;
}

template <typename Iter> NEXTWEB_INLINE typename std::iterator_traits<Iter>::value_type
EnumerationImpl<Iter>::nextElement() const {
	return *(begin_++);
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename std::iterator_traits<Iter>::value_type>::Pointer
makeEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename std::iterator_traits<Iter>::value_type>::Pointer(new EnumerationImpl<Iter>(begin, end));
}

}} // namespaces

#endif // NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED
