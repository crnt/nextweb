/** @file Response.hpp */
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

#ifndef NEXTWEB_RESPONSE_HPP_INCLUDED
#define NEXTWEB_RESPONSE_HPP_INCLUDED

#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/StandardSetup.hpp"
#include "nextweb/DetailsForward.hpp"

namespace nextweb {

class Server;
template <typename Setup> class Cookie;

class NEXTWEB_API Response {

public:
	virtual ~Response();
	
	void setStatus(int status);
	void setCookie(Cookie<StandardSetup> const &cookie);
	
	std::string const& getHeader(std::string const &name) const;
	void setHeader(std::string const &name, std::string const &value);
	
	std::size_t write(char const *buffer);
	std::size_t write(char const *buffer, std::size_t len);

private:
	Response(Response const &);
	Response& operator = (Response const &);
	friend class Server;

	typedef details::RequestImpl<StandardSetup> ImplType;
	Response(SharedPtr<ImplType> const &impl);

private:
	SharedPtr<ImplType> impl_;	
};

} // namespace

#endif // NEXTWEB_RESPONSE_HPP_INCLUDED
