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

#ifndef NEXTWEB_FASTCGI_GENERIC_RESPONSE_HPP_INCLUDED
#define NEXTWEB_FASTCGI_GENERIC_RESPONSE_HPP_INCLUDED

#include <map>
#include <set>
#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/fastcgi/Cookie.hpp"
#include "nextweb/fastcgi/HttpDate.hpp"

namespace nextweb { namespace fastcgi {

struct CookieLess {
	bool operator () (Cookie const &target, Cookie const &other) const;
};

template <typename IO>
class GenericResponse {

public:
	GenericResponse(IO &io);
	virtual ~GenericResponse();
	
	void setCookie(Cookie const &cookie);
	void setStatus(unsigned short status);
	void setHeader(std::string const &name, std::string const &value);
	
	void redirect(std::string const &url);
	void setExpireTime(HttpDate const &expires);
	void setExpireTime(std::string const &expires);
	std::size_t write(char const *buffer, std::size_t size);

private:
	GenericResponse(GenericResponse const &);
	GenericResponse& operator = (GenericResponse const &);
	
private:	
	IO &io_;
	HttpDate expireTime_;
	unsigned short httpStatus_;
	std::set<Cookie, CookieLess> cookies_;
	std::map<std::string, std::string> headers_;
};

template <typename IO> NEXTWEB_INLINE
 GenericResponse<IO>::GenericResponse(IO &io) :
	io_(io)
{
}

template <typename IO> NEXTWEB_INLINE 
GenericResponse<IO>::~GenericResponse() {
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setCookie(Cookie const &cookie) {
	cookies_.insert(cookie);
}
 
template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setStatus(unsigned short status) {
}
 
template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setHeader(std::string const &name, std::string const &value) {
	headers_[name] = value;
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::redirect(std::string const &url) {
	setStatus(301);
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setExpireTime(HttpDate const &expires) {
	expireTime_ = expires;
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setExpireTime(std::string const &expires) {
}

template <typename IO> NEXTWEB_INLINE std::size_t
GenericResponse<IO>::write(char const *buffer, std::size_t size) {
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_GENERIC_RESPONSE_HPP_INCLUDED
