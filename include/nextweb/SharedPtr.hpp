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

#ifndef NEXTWEB_SHARED_PTR_HPP_INCLUDED
#define NEXTWEB_SHARED_PTR_HPP_INCLUDED

#include "nextweb/Config.hpp"

#include <cassert>
#include <algorithm>

namespace nextweb {

template <typename T> 
class NEXTWEB_API SharedPtr {

public:
	explicit SharedPtr(T *value = 0);
	virtual ~SharedPtr();
	
	SharedPtr(SharedPtr<T> const &rhs);
	template <typename Other> SharedPtr(SharedPtr<Other> const &rhs);
	
	SharedPtr<T>& operator = (SharedPtr<T> const &rhs);
	template <typename Other> SharedPtr<T>& operator = (SharedPtr<Other> const &rhs);

	T* get();
	T* operator -> ();
	
	T const* get() const;
	T const* operator -> () const;
	
	T& operator * ();
	T const& operator * () const;
	
	void reset(T *value = 0);
	void swap(SharedPtr<T> &ptr) throw ();
	
	class BoolConvertible;
	operator BoolConvertible const* () const;
	
	template <typename Other> friend class SharedPtr;
	template <typename Other> SharedPtr<Other> cast() const;

private:
	T *value_;
};

template <typename T> NEXTWEB_INLINE
SharedPtr<T>::SharedPtr(T *value) :
	value_(value)
{
	if (0 != value_) incRef(value_);
}

template <typename T> NEXTWEB_INLINE
SharedPtr<T>::~SharedPtr() {
	if (0 != value_) decRef(value_);
}

template <typename T> NEXTWEB_INLINE
SharedPtr<T>::SharedPtr(SharedPtr<T> const &rhs) :
	value_(rhs.value_)
{
	if (0 != value_) incRef(value_);
}

template <typename T> template <typename Other> NEXTWEB_INLINE 
SharedPtr<T>::SharedPtr(SharedPtr<Other> const &rhs) :
	value_(rhs.value_)
{
	if (0 != value_) incRef(value_);
}

template <typename T> NEXTWEB_INLINE SharedPtr<T>&
SharedPtr<T>::operator = (SharedPtr<T> const &rhs) {
	SharedPtr<T> tmp(rhs);
	swap(tmp);
	return *this;
}

template <typename T> template <typename Other> NEXTWEB_INLINE SharedPtr<T>&
SharedPtr<T>::operator = (SharedPtr<Other> const &rhs) {
	SharedPtr<T> tmp(rhs);
	swap(tmp);
	return *this;
}

template <typename T> NEXTWEB_INLINE T*
SharedPtr<T>::get() {
	return value_;
}

template <typename T> NEXTWEB_INLINE T*
SharedPtr<T>::operator -> () {
	assert(0 != value_);
	return value_;
}

template <typename T> NEXTWEB_INLINE T const*
SharedPtr<T>::get() const {
	return value_;
}

template <typename T> NEXTWEB_INLINE T const*
SharedPtr<T>::operator -> () const {
	assert(0 != value_);
	return value_;
}
	
template <typename T> NEXTWEB_INLINE T&
SharedPtr<T>::operator * () {
	assert(0 != value_);
	return *value_;
}

template <typename T> NEXTWEB_INLINE T const&
SharedPtr<T>::operator * () const {
	assert(0 != value_);
	return *value_;
}
	
template <typename T> NEXTWEB_INLINE void
SharedPtr<T>::reset(T *value) {
	SharedPtr<T> tmp(value);
	swap(tmp);
}

template <typename T> NEXTWEB_INLINE void
SharedPtr<T>::swap(SharedPtr<T> &ptr) throw () {
	std::swap(value_, ptr.value_);
}

template <typename T> NEXTWEB_INLINE 
SharedPtr<T>::operator typename SharedPtr<T>::BoolConvertible const* () const {
	return (0 == value_) ? 0 : reinterpret_cast<BoolConvertible const*>(value_);
}

template <typename T> template <typename Other> NEXTWEB_INLINE SharedPtr<Other>
SharedPtr<T>::cast() const {
	return SharedPtr<Other>(dynamic_cast<Other*>(value_));
}

template <typename T, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator == (SharedPtr<T> const &ptr, SharedPtr<Other> const &other) {
	return ptr.get() == other.get();
}

template <typename T, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator == (T const* const ptr, SharedPtr<Other> const &other) {
	return ptr == other.get();
}

template <typename T, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator == (SharedPtr<T> const &ptr, Other const* const other) {
	return ptr.get() == other;
}

template <typename T, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator != (SharedPtr<T> const &ptr, SharedPtr<Other> const &other) {
	return ptr.get() != other.get();
}

template <typename T, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator != (T const* const ptr, SharedPtr<Other> const &other) {
	return ptr != other.get();
}

template <typename T, typename Other> NEXTWEB_API NEXTWEB_INLINE bool
operator != (SharedPtr<T> const &ptr, Other const* const other) {
	return ptr.get() != other;
}

template <typename T> NEXTWEB_API NEXTWEB_INLINE void
swap(SharedPtr<T> &ptr, SharedPtr<T> &other) {
	ptr.swap(other);
}

} // namespace

#endif // NEXTWEB_SHARED_PTR_HPP_INCLUDED
