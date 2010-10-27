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

#ifndef NEXTWEB_FASTCGI_REQUEST_IMPL_HPP_INCLUDED
#define NEXTWEB_FASTCGI_REQUEST_IMPL_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/fastcgi/Request.hpp"
#include "nextweb/fastcgi/impl/FileImpl.hpp"
#include "nextweb/fastcgi/impl/GenericRequest.hpp"

namespace nextweb { namespace fastcgi {

template <typename IO>
class RequestImpl : public Request, private GenericRequest<IO> {

public:
	RequestImpl();
	virtual ~RequestImpl();

	virtual bool isSecure() const;
	virtual std::string const& pathInfo() const;

	virtual Enumeration<Request::Param>::Pointer vars() const;
	virtual bool hasVar(std::string const &name) const;
	virtual std::string const& getVar(std::string const &name) const;

	virtual Enumeration<Request::Param>::Pointer cookies() const;
	virtual bool hasCookie(std::string const &name) const;
	virtual std::string const& getCookie(std::string const &name) const;

	virtual Enumeration<Request::Param>::Pointer args() const;
	virtual bool hasArg(std::string const &name) const;
	virtual std::string const& getArg(std::string const &name) const;
	virtual Enumeration<std::string>::Pointer argList(std::string const &name) const;

	virtual bool hasHeader(std::string const &name) const;
	virtual std::string const& getHeader(std::string const &name) const;

	virtual Enumeration<File>::Pointer files() const;
	virtual bool hasFile(std::string const &name) const;
	virtual File getFile(std::string const &name) const;
	
private:
	RequestImpl(RequestImpl const &);
	RequestImpl& operator = (RequestImpl const &);
};

template <typename IO> NEXTWEB_INLINE
RequestImpl<IO>::RequestImpl() 
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

template <typename IO> NEXTWEB_INLINE Enumeration<Request::Param>::Pointer
RequestImpl<IO>::vars() const {
	
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasVar(std::string const &name) const {
	return GenericRequest<IO>::hasVar(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getVar(std::string const &name) const {
	return GenericRequest<IO>::getVar(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<Request::Param>::Pointer
RequestImpl<IO>::cookies() const {
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasCookie(std::string const &name) const {
	return GenericRequest<IO>::hasCookie(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getCookie(std::string const &name) const {
	return GenericRequest<IO>::getCookie(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<Request::Param>::Pointer
RequestImpl<IO>::args() const {
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasArg(std::string const &name) const {
	return GenericRequest<IO>::hasArg(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getArg(std::string const &name) const {
	return GenericRequest<IO>::getArg(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<std::string>::Pointer
RequestImpl<IO>::argList(std::string const &name) const {
	typename GenericRequest<IO>::StringMultiMap const &map = GenericRequest<IO>::args();
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasHeader(std::string const &name) const {
	return GenericRequest<IO>::hasHeader(name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
RequestImpl<IO>::getHeader(std::string const &name) const {
	return GenericRequest<IO>::getHeader(name);
}

template <typename IO> NEXTWEB_INLINE Enumeration<File>::Pointer
RequestImpl<IO>::files() const {
}

template <typename IO> NEXTWEB_INLINE bool
RequestImpl<IO>::hasFile(std::string const &name) const {
}

template <typename IO> NEXTWEB_INLINE File
RequestImpl<IO>::getFile(std::string const &name) const {
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_REQUEST_IMPL_HPP_INCLUDED
