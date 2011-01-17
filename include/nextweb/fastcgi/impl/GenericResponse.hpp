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
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/StringConverters.hpp"

#include "nextweb/fastcgi/Cookie.hpp"
#include "nextweb/fastcgi/HttpDate.hpp"
#include "nextweb/fastcgi/HttpStatus.hpp"
#include "nextweb/fastcgi/impl/HttpUtils.hpp"

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
	void setHeader(std::string const &name, std::string const &value);
	void setHttpStatus(HttpStatus const &status);
	void redirect(std::string const &url);

	void setExpireTime(HttpDate const &exp);
	void setExpireTime(std::string const &exp);
	std::size_t write(char const *buffer, std::size_t size);

	void clear();

private:
	GenericResponse(GenericResponse const &);
	GenericResponse& operator = (GenericResponse const &);
	
	typedef std::set<Cookie, CookieLess> CookieSet;
	typedef std::map<std::string, std::string, utils::CILess<std::string>::Type> HeaderMap;
	
	void sendHeaders();
	
private:	
	IO &io_;
	bool headersSent_;
	CookieSet cookies_;
	HeaderMap headers_;
	HttpDate expireTime_;
	HttpStatus httpStatus_;
};

template <typename IO> NEXTWEB_INLINE
GenericResponse<IO>::GenericResponse(IO &io) :
	io_(io), httpStatus_(HttpStatus::BAD)
{
	clear();
}

template <typename IO> NEXTWEB_INLINE 
GenericResponse<IO>::~GenericResponse() {
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setCookie(Cookie const &cookie) {
	cookies_.insert(cookie);
	headersSent_ = false;
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setHeader(std::string const &name, std::string const &value) {
	if (utils::isCIEqual(HttpConstants::STATUS, name)) {
		setHttpStatus(HttpStatus::fromString(value));
	}
	else if (utils::isCIEqual(HttpConstants::EXPIRES, name)) {
		setExpireTime(value);
	}
	else {
		headers_[name] = value;
		headersSent_ = false;
	}
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setHttpStatus(HttpStatus const& status) {
	httpStatus_ = status;
	headersSent_ = false;
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::redirect(std::string const &url) {
	setHeader(HttpConstants::LOCATION, url);
	setHttpStatus(HttpStatus::MOVED_TEMPORARILY);
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setExpireTime(HttpDate const &exp) {
	expireTime_ = exp;
	headersSent_ = false;
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::setExpireTime(std::string const &exp) {
	setExpireTime(HttpDate::fromString(exp));
}

template <typename IO> NEXTWEB_INLINE std::size_t
GenericResponse<IO>::write(char const *buffer, std::size_t size) {
	if (HttpStatus::NO_CONTENT == httpStatus_) {
		throw Error("trying to write content while http status doen not allow it");
	}
	if (!headersSent_) {
		sendHeaders();
	}
	return io_.write(buffer, size);
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::clear() {
	cookies_.clear();
	headers_.clear();
	headersSent_ = false;
	httpStatus_ = HttpStatus::BAD;
	expireTime_.swap(HttpDate::BAD);
}

template <typename IO> NEXTWEB_INLINE void
GenericResponse<IO>::sendHeaders() {
	if (HttpStatus::BAD == httpStatus_) {
		throw Error("http status has not set explicitly");
	}
	io_.setStatus(httpStatus_.code());
	io_.writeHeader(HttpConstants::STATUS, httpStatus_.message());
	if (HttpDate::BAD != expireTime_) {
		io_.writeHeader(HttpConstants::EXPIRES, expireTime_.str());
	}
	for (CookieSet::const_iterator i = cookies_.begin(), end = cookies_.end(); i != end; ++i) {
		io_.writeHeader(HttpConstants::SET_COOKIE, i->str());
	}
	for (HeaderMap::const_iterator i = headers_.begin(), end = headers_.end(); i != end; ++i) {
		io_.writeHeader(i->first, i->second);
	}
	clear();
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/fastcgi/GenericResponse.hpp"
#endif

#endif // NEXTWEB_FASTCGI_GENERIC_RESPONSE_HPP_INCLUDED
