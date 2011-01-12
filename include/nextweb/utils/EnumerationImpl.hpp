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

#ifndef NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED
#define NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"

namespace nextweb { namespace utils {

template <typename Iter>
struct Identity {
	typedef Identity<Iter> Type;
	typedef typename std::iterator_traits<Iter>::value_type ValueType;
	static ValueType const& getValue(Iter iter);
};

template <typename Iter>
struct MapSelectFirst {
	typedef MapSelectFirst<Iter> Type;
	typedef typename std::iterator_traits<Iter>::value_type::first_type ValueType;
	static ValueType const& getValue(Iter iter);
};

template <typename Iter>
struct MapSelectSecond {
	typedef MapSelectSecond<Iter> Type;
	typedef typename std::iterator_traits<Iter>::value_type::second_type ValueType;
	static ValueType const& getValue(Iter iter);
};

template <typename Iter, typename Filter = typename Identity<Iter>::Type>
class IterEnumerationImpl : public Enumeration<typename Filter::ValueType const&> {

public:
	IterEnumerationImpl(Iter begin, Iter end);
	virtual ~IterEnumerationImpl();
	
	virtual bool hasMoreElements() const;
	virtual typename Filter::ValueType const& nextElement() const;

private:
	IterEnumerationImpl(IterEnumerationImpl const &);
	IterEnumerationImpl& operator = (IterEnumerationImpl const &);

private:
	Iter const end_;
	Iter mutable begin_;
};

template <typename Iter> NEXTWEB_INLINE typename Identity<Iter>::ValueType const&
Identity<Iter>::getValue(Iter iter) {
	return *iter;
}

template <typename Iter> NEXTWEB_INLINE typename MapSelectFirst<Iter>::ValueType const&
MapSelectFirst<Iter>::getValue(Iter iter) {
	return iter->first;
}

template <typename Iter> NEXTWEB_INLINE typename MapSelectSecond<Iter>::ValueType const&
MapSelectSecond<Iter>::getValue(Iter iter) {
	return iter->second;
}

template <typename Iter, typename Filter> NEXTWEB_INLINE
IterEnumerationImpl<Iter, Filter>::IterEnumerationImpl(Iter begin, Iter end) :
	end_(end), begin_(begin)
{
}

template <typename Iter, typename Filter> NEXTWEB_INLINE 
IterEnumerationImpl<Iter, Filter>::~IterEnumerationImpl() {
}

template <typename Iter, typename Filter> NEXTWEB_INLINE bool
IterEnumerationImpl<Iter, Filter>::hasMoreElements() const {
	return begin_ != end_;
}

template <typename Iter, typename Filter> NEXTWEB_INLINE typename Filter::ValueType const&
IterEnumerationImpl<Iter, Filter>::nextElement() const {
	return Filter::getValue(begin_++);
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename Identity<Iter>::ValueType const&>::Pointer
makeIterEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename Identity<Iter>::ValueType const&>::Pointer(new IterEnumerationImpl<Iter, typename Identity<Iter>::Type>(begin, end));
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename MapSelectFirst<Iter>::ValueType const&>::Pointer
makeSelectFirstEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename MapSelectFirst<Iter>::ValueType const&>::Pointer(new IterEnumerationImpl<Iter, typename MapSelectFirst<Iter>::Type>(begin, end));
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename MapSelectSecond<Iter>::ValueType const&>::Pointer
makeSelectSecondEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename MapSelectSecond<Iter>::ValueType const&>::Pointer(new IterEnumerationImpl<Iter, typename MapSelectSecond<Iter>::Type>(begin, end));
}

}} // namespaces

#endif // NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED
