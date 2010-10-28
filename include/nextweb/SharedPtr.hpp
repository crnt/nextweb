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

#ifndef NEXTWEB_SHARED_PTR_HPP_INCLUDED
#define NEXTWEB_SHARED_PTR_HPP_INCLUDED

#include "nextweb/Config.hpp"

#include <cassert>
#include <algorithm>

namespace nextweb {

template <typename Type> 
class NEXTWEB_API SharedPtr {

public:
	explicit SharedPtr(Type *value = 0);
	virtual ~SharedPtr();
	
	SharedPtr(SharedPtr<Type> const &rhs);
	template <typename Other> SharedPtr(SharedPtr<Other> const &rhs);
	
	SharedPtr<Type>& operator = (SharedPtr<Type> const &rhs);
	template <typename Other> SharedPtr<Type>& operator = (SharedPtr<Other> const &rhs);

	Type* get();
	Type* operator -> ();
	
	Type const* get() const;
	Type const* operator -> () const;
	
	Type& operator * ();
	Type const& operator * () const;
	
	void reset(Type *value = 0);
	void swap(SharedPtr<Type> &ptr) throw ();
	
	class BoolConvertible;
	operator BoolConvertible const* () const;
	
	template <typename Other> friend class SharedPtr;
	template <typename Other> SharedPtr<Other> cast() const;

private:
	Type *value_;
};

template <typename Type> NEXTWEB_INLINE
SharedPtr<Type>::SharedPtr(Type *value) :
	value_(value)
{
	if (0 != value_) incRef(value_);
}

template <typename Type> NEXTWEB_INLINE
SharedPtr<Type>::~SharedPtr() {
	if (0 != value_) decRef(value_);
}

template <typename Type> NEXTWEB_INLINE
SharedPtr<Type>::SharedPtr(SharedPtr<Type> const &rhs) :
	value_(rhs.value_)
{
	if (0 != value_) incRef(value_);
}

template <typename Type> template <typename Other> NEXTWEB_INLINE 
SharedPtr<Type>::SharedPtr(SharedPtr<Other> const &rhs) :
	value_(rhs.value_)
{
	if (0 != value_) incRef(value_);
}

template <typename Type> NEXTWEB_INLINE SharedPtr<Type>&
SharedPtr<Type>::operator = (SharedPtr<Type> const &rhs) {
	SharedPtr tmp(rhs);
	swap(tmp);
	return *this;
}

template <typename Type> template <typename Other> NEXTWEB_INLINE SharedPtr<Type>&
SharedPtr<Type>::operator = (SharedPtr<Other> const &rhs) {
	SharedPtr tmp(rhs);
	swap(tmp);
	return *this;
}

template <typename Type> NEXTWEB_INLINE Type*
SharedPtr<Type>::get() {
	return value_;
}

template <typename Type> NEXTWEB_INLINE Type*
SharedPtr<Type>::operator -> () {
	assert(0 != value_);
	return value_;
}

template <typename Type> NEXTWEB_INLINE Type const*
SharedPtr<Type>::get() const {
	return value_;
}

template <typename Type> NEXTWEB_INLINE Type const*
SharedPtr<Type>::operator -> () const {
	assert(0 != value_);
	return value_;
}
	
template <typename Type> NEXTWEB_INLINE Type&
SharedPtr<Type>::operator * () {
	assert(0 != value_);
	return *value_;
}

template <typename Type> NEXTWEB_INLINE Type const&
SharedPtr<Type>::operator * () const {
	assert(0 != value_);
	return *value_;
}
	
template <typename Type> NEXTWEB_INLINE void
SharedPtr<Type>::reset(Type *value) {
	SharedPtr<Type> tmp(value);
	swap(tmp);
}

template <typename Type> NEXTWEB_INLINE void
SharedPtr<Type>::swap(SharedPtr<Type> &ptr) throw () {
	std::swap(value_, ptr.value_);
}

template <typename Type> NEXTWEB_INLINE 
SharedPtr<Type>::operator typename SharedPtr<Type>::BoolConvertible const* () const {
	return (0 == value_) ? 0 : reinterpret_cast<BoolConvertible const*>(value_);
}

template <typename Type> template <typename Other> NEXTWEB_INLINE SharedPtr<Other>
SharedPtr<Type>::cast() const {
	return SharedPtr<Other>(dynamic_cast<Other*>(value_));
}

template <typename Type, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator == (SharedPtr<Type> const &ptr, SharedPtr<Other> const &other) {
	return ptr.get() == other.get();
}

template <typename Type, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator == (Type const* const ptr, SharedPtr<Other> const &other) {
	return ptr == other.get();
}

template <typename Type, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator == (SharedPtr<Type> const &ptr, Other const* const other) {
	return ptr.get() == other;
}

template <typename Type, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator != (SharedPtr<Type> const &ptr, SharedPtr<Other> const &other) {
	return ptr.get() != other.get();
}

template <typename Type, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator != (Type const* const ptr, SharedPtr<Other> const &other) {
	return ptr != other.get();
}

template <typename Type, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator != (SharedPtr<Type> const &ptr, Other const* const other) {
	return ptr.get() != other;
}

template <typename Type> NEXTWEB_API NEXTWEB_INLINE void
swap(SharedPtr<Type> &ptr, SharedPtr<Type> &other) {
	ptr.swap(other);
}

} // namespace

#endif // NEXTWEB_SHARED_PTR_HPP_INCLUDED
