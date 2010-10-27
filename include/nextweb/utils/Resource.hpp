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

#ifndef NEXTWEB_UTILS_RESOURCE_HPP_INCLUDED
#define NEXTWEB_UTILS_RESOURCE_HPP_INCLUDED

#include <algorithm>

#include "nextweb/Config.hpp"

namespace nextweb { namespace utils {

template <typename Type, typename Traits>
class Resource {

public:
	Resource();
	explicit Resource(Type value, Traits const &traits = Traits());
	virtual ~Resource();
	
	Type release();
	void reset(Type value);
	
	Type get();
	Type const get() const;

	class BoolConvertible;
	operator BoolConvertible const* () const;
	
private:
	Resource(Resource const &);
	Resource& operator = (Resource const &);
	void swap(Resource<Type, Traits> &other) throw ();
	
private:
	Type value_;
	Traits traits_;
};

template <typename Type, typename Traits> NEXTWEB_INLINE
Resource<Type, Traits>::Resource() :
	value_(Traits::defaultValue()), traits_()
{
}

template <typename Type, typename Traits> NEXTWEB_INLINE 
Resource<Type, Traits>::Resource(Type value, Traits const &traits) :
	value_(value), traits_(traits)
{
}

template <typename Type, typename Traits> NEXTWEB_INLINE 
Resource<Type, Traits>::~Resource() {
	if (Traits::defaultValue() != value_) {
		traits_.destroy(value_);
	}
}

template <typename Type, typename Traits> NEXTWEB_INLINE Type
Resource<Type, Traits>::release() {
	Type result = Traits::defaultValue();
	std::swap(value_, result);
	return result;
}

template <typename Type, typename Traits> NEXTWEB_INLINE void
Resource<Type, Traits>::reset(Type value) {
	Resource<Type, Traits> tmp(value, traits_);
	swap(tmp);
}

template <typename Type, typename Traits> NEXTWEB_INLINE Type
Resource<Type, Traits>::get() {
	return value_;
}

template <typename Type, typename Traits> NEXTWEB_INLINE Type const
Resource<Type, Traits>::get() const {
	return value_;
}

template <typename Type, typename Traits> NEXTWEB_INLINE
Resource<Type, Traits>::operator typename Resource<Type, Traits>::BoolConvertible const* () const {
	return Traits::defaultValue() != value_ ? reinterpret_cast<BoolConvertible const*>(this) : 0;
}

template <typename Type, typename Traits> NEXTWEB_INLINE void
Resource<Type, Traits>::swap(Resource<Type, Traits> &other) throw () {
	std::swap(value_, other.value_);
}

}} // namespaces

#endif // NEXTWEB_UTILS_RESOURCE_HPP_INCLUDED
