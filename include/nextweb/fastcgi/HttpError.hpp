// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef NEXTWEB_FASTCGI_HTTP_ERROR_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HTTP_ERROR_HPP_INCLUDED

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API HttpError : public Error {

public:
	HttpError(unsigned short code);
	virtual ~HttpError() throw ();
	
	unsigned short code() const;

	static unsigned short const BAD_REQUEST;
	static unsigned short const UNAUTHORIZED;
	static unsigned short const PAYMENT_REQUIRED;
	static unsigned short const FORBIDDEN;
	static unsigned short const NOT_FOUND;
	static unsigned short const METHOD_NOT_ALLOWED;
	static unsigned short const ENTITY_TOO_LARGE;
	static unsigned short const RANGE_NOT_SATISFIABLE;
	
	static unsigned short const INTERNAL_ERROR;
	static unsigned short const NOT_IMPLEMENTED;
	static unsigned short const BAD_GATEWAY;
	static unsigned short const SERVICE_UNAVAILABLE;
	static unsigned short const GATEWAY_TIMEOUT;
	static unsigned short const VERSION_NOT_SUPPORTED;
	
private:
	unsigned short code_;
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_HTTP_ERROR_HPP_INCLUDED
