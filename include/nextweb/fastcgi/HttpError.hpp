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

#ifndef NEXTWEB_FASTCGI_HTTP_ERROR_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HTTP_ERROR_HPP_INCLUDED

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"
#include "nextweb/fastcgi/HttpStatus.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API HttpError : public Error {

public:
	HttpError(HttpStatus const &code);
	virtual ~HttpError() throw ();
	HttpStatus status() const;
	
private:
	HttpStatus status_;
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_HTTP_ERROR_HPP_INCLUDED
