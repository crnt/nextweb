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

#include <cassert>

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"
#include "nextweb/utils/ReturnType.hpp"

namespace nextweb { namespace utils {

template <typename Type>
class SingleValueEnumeration : public Enumeration<typename ReturnType<Type>::Type> {

public:
	SingleValueEnumeration(Type const &value);
	virtual ~SingleValueEnumeration();

	virtual bool hasMoreElements() const;
	virtual typename ReturnType<Type>::Type nextElement() const;
	
	
private:
	SingleValueEnumeration(SingleValueEnumeration const &);
	SingleValueEnumeration& operator = (SingleValueEnumeration const &);

private:
	Type value_;
	bool mutable exhausted_;
};

template <typename Iter>
struct Identity {
	typedef Identity<Iter> Type;
	typedef typename ReturnType<typename std::iterator_traits<Iter>::value_type>::Type ValueType;
	static ValueType getValue(Iter iter);
};

template <typename Iter>
struct MapSelectFirst {
	typedef MapSelectFirst<Iter> Type;
	typedef typename ReturnType<typename std::iterator_traits<Iter>::value_type::first_type>::Type ValueType;
	static ValueType getValue(Iter iter);
};

template <typename Iter>
struct MapSelectSecond {
	typedef MapSelectSecond<Iter> Type;
	typedef typename ReturnType<typename std::iterator_traits<Iter>::value_type::second_type>::Type ValueType;
	static ValueType getValue(Iter iter);
};

template <typename Iter, typename Filter = typename Identity<Iter>::Type>
class IterEnumerationImpl : public Enumeration<typename Filter::ValueType> {

public:
	IterEnumerationImpl(Iter begin, Iter end);
	virtual ~IterEnumerationImpl();
	
	virtual bool hasMoreElements() const;
	virtual typename Filter::ValueType nextElement() const;

private:
	IterEnumerationImpl(IterEnumerationImpl const &);
	IterEnumerationImpl& operator = (IterEnumerationImpl const &);

private:
	Iter const end_;
	Iter mutable begin_;
};

template <typename Type> NEXTWEB_INLINE
SingleValueEnumeration<Type>::SingleValueEnumeration(Type const &value) :
	value_(value), exhausted_(false)
{
}

template <typename Type> NEXTWEB_INLINE
SingleValueEnumeration<Type>::~SingleValueEnumeration() {
}

template <typename Type> NEXTWEB_INLINE typename ReturnType<Type>::Type
SingleValueEnumeration<Type>::nextElement() const {
	assert(!exhausted_);
	exhausted_ = true;
	return value_;
}

template <typename Type> NEXTWEB_INLINE bool
SingleValueEnumeration<Type>::hasMoreElements() const {
	return !exhausted_;
}

template <typename Iter> NEXTWEB_INLINE typename Identity<Iter>::ValueType
Identity<Iter>::getValue(Iter iter) {
	return *iter;
}

template <typename Iter> NEXTWEB_INLINE typename MapSelectFirst<Iter>::ValueType
MapSelectFirst<Iter>::getValue(Iter iter) {
	return iter->first;
}

template <typename Iter> NEXTWEB_INLINE typename MapSelectSecond<Iter>::ValueType
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

template <typename Iter, typename Filter> NEXTWEB_INLINE typename Filter::ValueType
IterEnumerationImpl<Iter, Filter>::nextElement() const {
	assert(begin_ != end_);
	return Filter::getValue(begin_++);
}

template <typename Type> NEXTWEB_INLINE typename Enumeration<Type>::Pointer
makeSingleValueEnumeration(Type const &value) {
	return typename Enumeration<Type>::Pointer(new SingleValueEnumeration<Type>(value));
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename Identity<Iter>::ValueType>::Pointer
makeIterEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename Identity<Iter>::ValueType>::Pointer(new IterEnumerationImpl<Iter, typename Identity<Iter>::Type>(begin, end));
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename MapSelectFirst<Iter>::ValueType>::Pointer
makeSelectFirstEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename MapSelectFirst<Iter>::ValueType>::Pointer(new IterEnumerationImpl<Iter, typename MapSelectFirst<Iter>::Type>(begin, end));
}

template <typename Iter> NEXTWEB_INLINE typename Enumeration<typename MapSelectSecond<Iter>::ValueType>::Pointer
makeSelectSecondEnumeration(Iter begin, Iter end) {
	return typename Enumeration<typename MapSelectSecond<Iter>::ValueType>::Pointer(new IterEnumerationImpl<Iter, typename MapSelectSecond<Iter>::Type>(begin, end));
}

}} // namespaces

#endif // NEXTWEB_UTILS_ENUMERATION_IMPL_HPP_INCLUDED
