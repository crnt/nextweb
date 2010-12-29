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

#ifndef NEXTWEB_FASTCGI_HTTP_UTILS_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HTTP_UTILS_HPP_INCLUDED

#include <string>

#include "nextweb/utils/Range.hpp"

namespace nextweb { namespace fastcgi {

char const*
statusToString(unsigned short code);

struct HttpConstants {

	static std::string const GET;
	static std::string const HEAD;
	static std::string const POST;
	static std::string const METHOD;
	static std::string const EMPTY_STRING;
	static std::string const PATH_INFO_VAR_NAME;
	static std::string const CONTENT_TYPE_VAR_NAME;
	static std::string const CONTENT_LENGTH_VAR_NAME;
	
	static utils::Range<char const*> const NN;
	static utils::Range<char const*> const RNRN;
	static utils::Range<char const*> const BOUNDARY;
	static utils::Range<char const*> const FORM_DATA;
	static utils::Range<char const*> const MULTIPART_ENDING;
	static utils::Range<char const*> const MULTIPART_FORM_DATA;
	static utils::Range<char const*> const CONTENT_TYPE;
	static utils::Range<char const*> const CONTENT_DISP;
	static utils::Range<char const*> const NAME;
	static utils::Range<char const*> const FILE_NAME;

	
	static utils::Range<char const*> const COOKIE_VAR_NAME;
	static utils::Range<char const*> const QUERY_STRING_VAR_NAME;
};
	
std::string
makeVarName(std::string const &header);

}} // namespaces

#endif // NEXTWEB_FASTCGI_HTTP_UTILS_HPP_INCLUDED
