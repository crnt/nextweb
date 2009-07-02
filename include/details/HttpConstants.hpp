/** @file HttpConstants.hpp */
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

#ifndef NEXTWEB_DETAILS_HTTP_CONSTANTS_HPP_INCLUDED
#define NEXTWEB_DETAILS_HTTP_CONSTANTS_HPP_INCLUDED

#include "details/Range.hpp"

namespace nextweb { namespace details {

template <typename Setup>
struct HttpConstants {

	static char const* httpStatus(int status);
	
	static void convertStatus(char *buffer, std::size_t size, int status);

	static typename Setup::StringType const POST;
	
	static typename Setup::StringType const COOKIE;
	
	static typename Setup::StringType const QUERY_STRING;
	
	static typename Setup::StringType const CONTENT_LENGTH;
	
	static typename Setup::StringType const REQUEST_METHOD;

	static Range<char const*> const HTTP_PREFIX;
	
	static Range<char const*> const COOKIE_DELIMITER;
};

template <typename Setup> typename Setup::StringType const
HttpConstants<Setup>::POST("POST");

template <typename Setup> typename Setup::StringType const
HttpConstants<Setup>::COOKIE("HTTP_COOKIE");

template <typename Setup> typename Setup::StringType const
HttpConstants<Setup>::QUERY_STRING("QUERY_STRING");

template <typename Setup> typename Setup::StringType const
HttpConstants<Setup>::CONTENT_LENGTH("HTTP_CONTENT_LENGTH");

template <typename Setup> typename Setup::StringType const
HttpConstants<Setup>::REQUEST_METHOD("REQUEST_METHOD");

template <typename Setup> Range<char const*> const
HttpConstants<Setup>::HTTP_PREFIX = makeRange("HTTP_");

template <typename Setup> Range<char const*> const
HttpConstants<Setup>::COOKIE_DELIMITER = makeRange("; ");

template <typename Setup> inline char const*
HttpConstants<Setup>::httpStatus(int status) {
	
	switch (status) {
		
		case 200: return "Ok";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Partial information";
		case 204: return "No response";
		case 205: return "Content created";

		case 301: return "Moved";
		case 302: return "Moved temporarily";
		case 303: return "Method";
		case 304: return "Not Modified";

		case 400: return "Bad request";
		case 401: return "Unauthorized";
		case 402: return "Payment required";
		case 403: return "Forbidden";
		case 404: return "Not found";
		case 405: return "Method not allowed";

		case 500: return "Internal error";
		case 501: return "Not implemented";
		case 502: return "Service temporarily overloaded";
		case 503: return "Gateway timeout";
	}
	return "Unknown status";
}

}} // namespaces

#endif // NEXTWEB_DETAILS_HTTP_CONSTANTS_HPP_INCLUDED
