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

#ifndef NEXTWEB_FASTCGI_REQUEST_IMPL_HPP_INCLUDED
#define NEXTWEB_FASTCGI_REQUEST_IMPL_HPP_INCLUDED

#include <iterator>

#include "nextweb/Config.hpp"
#include "nextweb/utils/Iterator.hpp"
#include "nextweb/utils/EnumerationImpl.hpp"

#include "nextweb/fastcgi/Request.hpp"
#include "nextweb/fastcgi/impl/FileImpl.hpp"
#include "nextweb/fastcgi/impl/GenericRequest.hpp"

namespace nextweb { namespace fastcgi {

template <typename Map>
class UniqueKeyIterator : public utils::Iterator<UniqueKeyIterator<Map>, std::forward_iterator_tag, typename Map::key_type const> {

public:
	UniqueKeyIterator(typename Map::const_iterator begin, typename Map::const_iterator end);

	void increment();
	typename Map::key_type const& dereference();
	bool equal(UniqueKeyIterator<Map> const &other) const;

private:
	typename Map::const_iterator i_;
	typename Map::const_iterator const end_;
};

template <typename IO>
class RequestImpl : public Request, private GenericRequest<IO> {

public:
	RequestImpl(IO &io, std::size_t threshold);
	virtual ~RequestImpl();

	virtual bool isSecure() const;
	virtual std::string const& pathInfo() const;

	virtual bool hasVar(std::string const &name) const;
	virtual std::string const& getVar(std::string const &name) const;
	virtual Enumeration<Request::Param const&>::Pointer getVars() const;
	virtual Enumeration<std::string const&>::Pointer getVarNames() const;

	virtual bool hasCookie(std::string const &name) const;
	virtual std::string const& getCookie(std::string const &name) const;
	virtual Enumeration<Request::Param const&>::Pointer getCookies() const;
	virtual Enumeration<std::string const&>::Pointer getCookieNames() const;

	virtual bool hasArg(std::string const &name) const;
	virtual std::string const& getArg(std::string const &name) const;
	virtual Enumeration<Request::Param const&>::Pointer getArgs() const;
	virtual Enumeration<std::string const&>::Pointer getArgList(std::string const &name) const;
	virtual Enumeration<std::string const&>::Pointer getArgNames() const;

	virtual bool hasHeader(std::string const &name) const;
	virtual std::string const& getHeader(std::string const &name) const;

	virtual bool hasFile(std::string const &name) const;
	virtual File getFile(std::string const &name) const;
	virtual Enumeration<Request::FileParam const&>::Pointer getFiles() const;
	virtual Enumeration<std::string const&>::Pointer getFileNames() const;

	virtual void store(char const *file);
	
private:
	RequestImpl(RequestImpl const &);
	RequestImpl& operator = (RequestImpl const &);
};

template <typename Map> NEXTWEB_INLINE 
UniqueKeyIterator<Map>::UniqueKeyIterator(typename Map::const_iterator begin, typename Map::const_iterator end) :
	i_(begin), end_(end)
{
}

template <typename Map> NEXTWEB_INLINE void
UniqueKeyIterator<Map>::increment() {
	typename Map::const_iterator copy = i_;
	while (end_ != i_ && (i_->first == copy->first)) {
		++i_;
	}
}

template <typename Map> NEXTWEB_INLINE bool
UniqueKeyIterator<Map>::equal(UniqueKeyIterator<Map> const &other) const {
	return (i_ == other.i_);
}

template <typename Map> NEXTWEB_INLINE typename Map::key_type const&
UniqueKeyIterator<Map>::dereference() {
	return i_->first;
}

template <typename IO> NEXTWEB_INLINE
RequestImpl<IO>::RequestImpl(IO &io, std::size_t threshold) :
	GenericRequest<IO>::GenericRequest(io, threshold)
{
}

template <typename IO> NEXTWEB_INLINE
RequestImpl<IO>::~RequestImpl() {
}

template <typename IO> NEXTWEB_INLINE bool 
RequestImpl<IO>::isSecure() const {
	return GenericRequest<IO>::isSecure();
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::pathInfo() const {
	return GenericRequest<IO>::pathInfo();
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasVar(std::string const &name) const {
	return GenericRequest<IO>::hasVar(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getVar(std::string const &name) const {
	return GenericRequest<IO>::getVar(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<Request::Param const&>::Pointer
RequestImpl<IO>::getVars() const {
	typename GenericRequest<IO>::StringMap const &m = GenericRequest<IO>::vars();
	return utils::makeIterEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE Enumeration<std::string const&>::Pointer
RequestImpl<IO>::getVarNames() const {
	typename GenericRequest<IO>::StringMap const &m = GenericRequest<IO>::vars();
	return utils::makeSelectFirstEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasCookie(std::string const &name) const {
	return GenericRequest<IO>::hasCookie(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getCookie(std::string const &name) const {
	return GenericRequest<IO>::getCookie(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<Request::Param const&>::Pointer
RequestImpl<IO>::getCookies() const {
	typename GenericRequest<IO>::StringMap const &m = GenericRequest<IO>::cookies();
	return utils::makeIterEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE Enumeration<std::string const&>::Pointer
RequestImpl<IO>::getCookieNames() const {
	typename GenericRequest<IO>::StringMap const &m = GenericRequest<IO>::cookies();
	return utils::makeSelectFirstEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasArg(std::string const &name) const {
	return GenericRequest<IO>::hasArg(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getArg(std::string const &name) const {
	return GenericRequest<IO>::getArg(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<Request::Param const&>::Pointer
RequestImpl<IO>::getArgs() const {
	typename GenericRequest<IO>::StringMultiMap const &m = GenericRequest<IO>::args();
	return utils::makeIterEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE Enumeration<std::string const&>::Pointer
RequestImpl<IO>::getArgList(std::string const &name) const {
	typedef typename GenericRequest<IO>::StringMultiMap MapType;
	MapType const &m = GenericRequest<IO>::args();
	std::pair<typename MapType::const_iterator, typename MapType::const_iterator> p = m.equal_range(name);
	return utils::makeSelectSecondEnumeration(p.first, p.second);
}

template <typename IO> NEXTWEB_INLINE Enumeration<std::string const&>::Pointer
RequestImpl<IO>::getArgNames() const {
	typedef typename GenericRequest<IO>::StringMultiMap MapType;
	MapType const &m = GenericRequest<IO>::args();
	UniqueKeyIterator<MapType> begin(m.begin(), m.end()), end(m.end(), m.end());
	return utils::makeIterEnumeration(begin, end);
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasFile(std::string const &name) const {
	return GenericRequest<IO>::hasFile(name);
}

template <typename IO> NEXTWEB_INLINE File
RequestImpl<IO>::getFile(std::string const &name) const {
	return GenericRequest<IO>::getFile(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<Request::FileParam const&>::Pointer
RequestImpl<IO>::getFiles() const {
	typename GenericRequest<IO>::FileMap const &m = GenericRequest<IO>::files();
	return utils::makeIterEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE Enumeration<std::string const&>::Pointer
RequestImpl<IO>::getFileNames() const {
	typename GenericRequest<IO>::FileMap const &m = GenericRequest<IO>::files();
	return utils::makeSelectFirstEnumeration(m.begin(), m.end());
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasHeader(std::string const &name) const {
	return GenericRequest<IO>::hasHeader(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getHeader(std::string const &name) const {
	return GenericRequest<IO>::getHeader(name);
}

template <typename IO> NEXTWEB_INLINE void
RequestImpl<IO>::store(char const *file) {
	GenericRequest<IO>::store(file);
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_REQUEST_IMPL_HPP_INCLUDED
