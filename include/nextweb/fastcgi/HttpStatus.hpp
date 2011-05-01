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

#ifndef NEXTWEB_FASTCGI_HTTP_STATUS_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HTTP_STATUS_HPP_INCLUDED

#include <string>

#include "nextweb/Config.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API HttpStatus {

public:
	explicit HttpStatus(unsigned short code);
	
	unsigned short code() const;
	char const* message() const;
	
	bool operator == (HttpStatus const &other) const;
	bool operator != (HttpStatus const &other) const;

	static HttpStatus const OK;
	static HttpStatus const CREATED;
	static HttpStatus const ACCEPTED;
	static HttpStatus const NON_AUTHORITATIVE;
	static HttpStatus const NO_CONTENT;
	
	static HttpStatus const MOVED_PERMANENTLY;
	static HttpStatus const MOVED_TEMPORARILY;
	static HttpStatus const SEE_OTHER;
	static HttpStatus const NOT_MODIFIED;

	static HttpStatus const BAD_REQUEST;
	static HttpStatus const UNAUTHORIZED;
	static HttpStatus const PAYMENT_REQUIRED;
	static HttpStatus const FORBIDDEN;
	static HttpStatus const NOT_FOUND;
	static HttpStatus const METHOD_NOT_ALLOWED;
	static HttpStatus const ENTITY_TOO_LARGE;
	static HttpStatus const RANGE_NOT_SATISFIABLE;
	
	static HttpStatus const INTERNAL_ERROR;
	static HttpStatus const NOT_IMPLEMENTED;
	static HttpStatus const BAD_GATEWAY;
	static HttpStatus const SERVICE_UNAVAILABLE;
	static HttpStatus const GATEWAY_TIMEOUT;
	static HttpStatus const VERSION_NOT_SUPPORTED;
	static HttpStatus const BAD;

private:
	unsigned short code_;
};

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/fastcgi/HttpStatus.hpp"
#endif 

#endif // NEXTWEB_FASTCGI_HTTP_STATUS_HPP_INCLUDED
