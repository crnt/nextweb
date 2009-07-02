/** @file Resource.hpp */
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

#ifndef NEXTWEB_DETAILS_RESOURCE_HPP_INCLUDED
#define NEXTWEB_DETAILS_RESOURCE_HPP_INCLUDED

#include <algorithm>

namespace nextweb { namespace details {

template <typename Type, typename Traits>
class Resource {

public:
	Resource();
	explicit Resource(Type value);
	Resource(Type value, Traits const &traits);
	virtual ~Resource();

	Type get();
	Type release();
	void reset(Type value);
	void swap(Resource<Type, Traits> &other);
	Type const get() const;

	class BoolConvertible;
	operator BoolConvertible const* () const;
	
private:
	Resource(Resource const &);
	Resource& operator = (Resource const &);
	
private:
	Type value_;
	Traits traits_;
};

template <typename Type, typename Traits> inline 
Resource<Type, Traits>::Resource() :
	value_(Traits::defaultValue()), traits_()
{
}

template <typename Type, typename Traits> inline 
Resource<Type, Traits>::Resource(Type value) :
	value_(value), traits_()
{
}

template <typename Type, typename Traits> inline 
Resource<Type, Traits>::Resource(Type value, Traits const &traits) :
	value_(value), traits_(traits)
{
}

template <typename Type, typename Traits> inline 
Resource<Type, Traits>::~Resource() {
	if (Traits::defaultValue() != value_) {
		traits_.destroy(value_);
	}
}

template <typename Type, typename Traits> inline Type
Resource<Type, Traits>::get() {
	return value_;
}

template <typename Type, typename Traits> inline Type
Resource<Type, Traits>::release() {
	Type result = value_;
	reset(Traits::defaultValue());
	return result;
}

template <typename Type, typename Traits> inline void
Resource<Type, Traits>::reset(Type value) {
	Resource<Type, Traits> tmp(value);
	swap(tmp);
}

template <typename Type, typename Traits> inline void
Resource<Type, Traits>::swap(Resource<Type, Traits> &other) {
	std::swap(value_, other.value_);
}

template <typename Type, typename Traits> inline Type const
Resource<Type, Traits>::get() const {
	return value_;
}

template <typename Type, typename Traits> inline
Resource<Type, Traits>::operator typename Resource<Type, Traits>::BoolConvertible const* () const {
	return Traits::defaultValue() != value_ ? reinterpret_cast<BoolConvertible const*>(this) : 0;
}

}} // namespaces

#endif // NEXTWEB_DETAILS_RESOURCE_HPP_INCLUDED
