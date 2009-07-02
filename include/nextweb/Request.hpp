/** @file Request.hpp */
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

#ifndef NEXTWEB_REQUEST_HPP_INCLUDED
#define NEXTWEB_REQUEST_HPP_INCLUDED

#include <string>
#include <utility>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/Enumeration.hpp"
#include "nextweb/StandardSetup.hpp"
#include "nextweb/DetailsForward.hpp"

namespace nextweb {

class NEXTWEB_API Request {

public:

	typedef StandardSetup::StringType StringType;
	typedef details::RequestImpl<StandardSetup> ImplType;
	typedef std::pair<const StringType, StringType> ParamType;
	
	Request(SharedPtr<ImplType> const &impl);
	virtual ~Request();

	bool hasArg(StringType const &name) const;
	StringType const& getArg(StringType const &name) const;
	Enumeration<ParamType const&>::PtrType args() const;
	Enumeration<StringType const&>::PtrType argNames() const;
	Enumeration<StringType const&>::PtrType argList(StringType const &name) const;
	
	bool hasVar(StringType const &name) const;
	StringType const& getVar(StringType const &name) const;
	Enumeration<ParamType const&>::PtrType vars() const;
	Enumeration<StringType const&>::PtrType varNames() const;
	
	bool hasCookie(StringType const &name) const;
	StringType const& getCookie(StringType const &name) const;
	Enumeration<ParamType const&>::PtrType cookies() const;
	Enumeration<StringType const&>::PtrType cookieNames() const;

	bool hasHttp(StringType const &name) const;
	StringType const& getHttp(StringType const &name) const;
	
private:
	Request(Request const &);
	Request& operator = (Request const &);
	

private:
	SharedPtr<ImplType> impl_;	
};

} // namespace

#endif // NEXTWEB_REQUEST_HPP_INCLUDED
