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

#ifndef NEXTWEB_FASTCGI_HANDLER_DATA_HPP_INCLUDED
#define NEXTWEB_FASTCGI_HANDLER_DATA_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/Shared.hpp"

namespace nextweb { namespace fastcgi {

template <typename Setup>
class HandlerData : public Shared, public Setup::IOType {
	
public: 
	HandlerData(int socket);
	virtual ~HandlerData();
	
	typedef typename Setup::IOType BaseType; 
	typedef typename Setup::RequestType RequestType;
	typedef typename Setup::ResponseType ResponseType;

	void accept();
	void finish();

	ResponseType& response();
	RequestType const& request() const;

private:
	HandlerData(HandlerData const &);
	HandlerData& operator = (HandlerData const &);

private:
	RequestType request_;
	ResponseType response_;
};

template <typename Setup> NEXTWEB_INLINE 
HandlerData<Setup>::HandlerData(int socket) :
	BaseType(socket), request_(*this), response_(*this)
{
}

template <typename Setup> NEXTWEB_INLINE 
HandlerData<Setup>::~HandlerData() {
}

template <typename Setup> NEXTWEB_INLINE void
HandlerData<Setup>::accept() {
	BaseType::accept();
	request_.parse(0);
}

template <typename Setup> NEXTWEB_INLINE void
HandlerData<Setup>::finish() {
	BaseType::finish();
	request_.clear();
	response_.clear();
}

template <typename Setup> NEXTWEB_INLINE typename HandlerData<Setup>::ResponseType&
HandlerData<Setup>::response() {
	return response_;
}

template <typename Setup> NEXTWEB_INLINE typename HandlerData<Setup>::RequestType const&
HandlerData<Setup>::request() const {
	return request_;
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_HANDLER_DATA_HPP_INCLUDED
