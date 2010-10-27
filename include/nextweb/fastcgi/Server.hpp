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

#ifndef NEXTWEB_FASTCGI_HTTP_SERVER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HTTP_SERVER_HPP_INCLUDED

#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/fastcgi/Forward.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API Server {

public:
	Server();
	virtual ~Server();

	void stop();
	void start(Settings const &set);
	void addHandler(std::string const &url, SharedPtr<RequestHandler> const &handler);

private:
	SharedPtr<ServerImpl> impl_;
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_HTTP_SERVER_HPP_INCLUDED
