/** @file EnumerationImpl.hpp */
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

#ifndef NEXTWEB_DETAILS_ENUMERATION_HPP_INCLUDED
#define NEXTWEB_DETAILS_ENUMERATION_HPP_INCLUDED

#include <iterator>

#include "nextweb/Enumeration.hpp"

namespace nextweb { namespace details {

template <typename Cont>
class EnumerationImpl : public Enumeration<typename Cont::value_type const&> {

public:
	EnumerationImpl(Cont const &cont);
	EnumerationImpl(typename Cont::const_iterator begin, typename Cont::const_iterator end);
	virtual ~EnumerationImpl();
	
	virtual bool hasMoreElements() const;
	virtual typename Cont::value_type const& nextElement() const;

private:
	EnumerationImpl(EnumerationImpl const &);
	EnumerationImpl& operator = (EnumerationImpl const &);

private:
	typename Cont::const_iterator end_;
	mutable typename Cont::const_iterator begin_;
};

template <typename Map>
class MapKeyEnumeration : public Enumeration<typename Map::key_type const&> {

public:
	MapKeyEnumeration(Map const &map);
	MapKeyEnumeration(typename Map::const_iterator begin, typename Map::const_iterator end);
	virtual ~MapKeyEnumeration();

	virtual bool hasMoreElements() const;
	virtual typename Map::key_type const& nextElement() const;

private:
	MapKeyEnumeration(MapKeyEnumeration const &);
	MapKeyEnumeration& operator = (MapKeyEnumeration const &);
	
private:
	typename Map::const_iterator end_;
	mutable typename Map::const_iterator begin_;
};

template <typename Cont> inline
EnumerationImpl<Cont>::EnumerationImpl(Cont const &cont) :
	end_(cont.end()), begin_(cont.begin())
{
}

template <typename Cont> inline
EnumerationImpl<Cont>::EnumerationImpl(typename Cont::const_iterator begin, typename Cont::const_iterator end) :
	end_(end), begin_(begin)
{
}

template <typename Cont> inline
EnumerationImpl<Cont>::~EnumerationImpl() {

}

template <typename Cont> inline bool
EnumerationImpl<Cont>::hasMoreElements() const {
	return begin_ != end_;
}

template <typename Cont> inline typename Cont::value_type const&
EnumerationImpl<Cont>::nextElement() const {
	return *(begin_++);
}

template <typename Map> inline
MapKeyEnumeration<Map>::MapKeyEnumeration(Map const &map) :
	end_(map.end()), begin_(map.begin())
{
}

template <typename Map> inline
MapKeyEnumeration<Map>::MapKeyEnumeration(typename Map::const_iterator begin, typename Map::const_iterator end) :
	end_(end), begin_(begin)
{
}

template <typename Map> inline
MapKeyEnumeration<Map>::~MapKeyEnumeration() {
}

template <typename Map> inline bool
MapKeyEnumeration<Map>::hasMoreElements() const {
	return begin_ != end_;
}

template <typename Map> inline typename Map::key_type const&
MapKeyEnumeration<Map>::nextElement() const {
	return (begin_++)->first;
}

template <typename Cont> inline typename Enumeration<typename Cont::value_type const&>::PtrType
makeEnumeration(Cont const &cont) {
	return typename Enumeration<typename Cont::value_type const&>::PtrType(new EnumerationImpl<Cont>(cont));
}

template <typename Cont> inline typename Enumeration<typename Cont::value_type const&>::PtrType
makeEnumeration(typename Cont::const_iterator begin, typename Cont::const_iterator end) {
	return typename Enumeration<typename Cont::value_type const&>::PtrType(new EnumerationImpl<Cont>(begin, end));
}

template <typename Map> inline typename Enumeration<typename Map::key_type const&>::PtrType
makeMapKeyEnumeration(Map const &map) {
	return typename Enumeration<typename Map::key_type const&>::PtrType(new MapKeyEnumeration<Map>(map));
}

}} // namespaces

#endif // NEXTWEB_DETAILS_ENUMERATION_HPP_INCLUDED
