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

#ifndef NEXTWEB_FASTCGI_RESPONSE_IMPL_HPP_INCLUDED
#define NEXTWEB_FASTCGI_RESPONSE_IMPL_HPP_INCLUDED

#include "nextweb/fastcgi/Response.hpp"
#include "nextweb/fastcgi/impl/GenericResponse.hpp"

namespace nextweb { namespace fastcgi {

template <typename IO>
class ResponseImpl : public Response, public GenericResponse<IO> {

public:
	ResponseImpl(IO &io);
	virtual ~ResponseImpl();

	virtual void setCookie(Cookie const &cookie);
	virtual void setStatus(unsigned short status);
	virtual void setHeader(std::string const &name, std::string const &value);
	
	virtual void redirect(std::string const &url);
	virtual void setExpireTime(HttpDate const &date);
	virtual void setExpireTime(std::string const &expires);
	virtual std::size_t write(char const *buffer, std::size_t size);

private:
	ResponseImpl(ResponseImpl const &);
	ResponseImpl& operator = (ResponseImpl const &);
};

template <typename IO> NEXTWEB_INLINE 
ResponseImpl<IO>::ResponseImpl(IO &io) :
	GenericResponse<IO>(io)
{
}

template <typename IO> NEXTWEB_INLINE 
ResponseImpl<IO>::~ResponseImpl() {
}

template <typename IO> NEXTWEB_INLINE void
ResponseImpl<IO>::setCookie(Cookie const &cookie) {
}

template <typename IO> NEXTWEB_INLINE void
ResponseImpl<IO>::setStatus(unsigned short status) {
}

template <typename IO> NEXTWEB_INLINE void
ResponseImpl<IO>::setHeader(std::string const &name, std::string const &value) {
}

template <typename IO> NEXTWEB_INLINE void
ResponseImpl<IO>::redirect(std::string const &url) {
}

template <typename IO> NEXTWEB_INLINE void
ResponseImpl<IO>::setExpireTime(HttpDate const &date) {
}

template <typename IO> NEXTWEB_INLINE void
ResponseImpl<IO>::setExpireTime(std::string const &expires) {
}

template <typename IO> NEXTWEB_INLINE std::size_t
ResponseImpl<IO>::write(char const *buffer, std::size_t size) {
}

}} // namespsaces

#endif // NEXTWEB_FASTCGI_RESPONSE_IMPL_HPP_INCLUDED
