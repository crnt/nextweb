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

#ifndef NEXTWEB_FASTCGI_REQUEST_HPP_INCLUDED
#define NEXTWEB_FASTCGI_REQUEST_HPP_INCLUDED

#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"
#include "nextweb/fastcgi/File.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API Request {

public:
	Request();
	virtual ~Request();

	typedef std::pair<std::string const, std::string> Param;

	virtual bool isSecure() const = 0;
	virtual std::string const& pathInfo() const = 0;

	virtual Enumeration<Param>::Pointer vars() const = 0;
	virtual bool hasVar(std::string const &name) const = 0;
	virtual std::string const& getVar(std::string const &name) const = 0;

	virtual Enumeration<Param>::Pointer cookies() const = 0;
	virtual bool hasCookie(std::string const &name) const = 0;
	virtual std::string const& getCookie(std::string const &name) const = 0;

	virtual Enumeration<Param>::Pointer args() const = 0;
	virtual bool hasArg(std::string const &name) const = 0;
	virtual std::string const& getArg(std::string const &name) const = 0;
	virtual Enumeration<std::string>::Pointer argList(std::string const &name) const = 0;

	virtual bool hasHeader(std::string const &name) const = 0;
	virtual std::string const& getHeader(std::string const &name) const = 0;

	virtual Enumeration<File>::Pointer files() const = 0;
	virtual bool hasFile(std::string const &name) const = 0;
	virtual File getFile(std::string const &name) const = 0;
	
private:
	Request(Request const &);
	Request& operator = (Request const &);
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_REQUEST_HPP_INCLUDED
