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

#ifndef NEXTWEB_FASTCGI_CONTEXT_HPP_INCLUDED
#define NEXTWEB_FASTCGI_CONTEXT_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/utils/Threads.hpp"

namespace nextweb { namespace fastcgi {

template <typename Setup>
class Context : public utils::AtomicShared {

public:
	Context();
	virtual ~Context();

	typedef typename Setup::IOType IOType;
	typedef typename Setup::RequestType RequestType;
	typedef typename Setup::ResponseType ResponseType;

	void accept();
	void finish();

	ResponseType& response();
	RequestType const& request() const;
	
private:
	Context(Context const &);
	Context& operator = (Context const &);

private:
	IOType io_;
	RequestType request_;
	ResponseType response_;
};

template <typename Setup> NEXTWEB_INLINE
Context<Setup>::Context() : 
	io_(), request_(io_), response_(io)
{
}

template <typename Setup> NEXTWEB_INLINE
Context<Setup>::~Context() {
}

template <typename Setup> NEXTWEB_INLINE ResponseType&
Context<Setup>::response() {
	return response_;
}

template <typename Setup> NEXTWEB_INLINE RequestType const&
Context<Setup>::request() const {
	return request_;
}

template <typename Setup> NEXTWEB_INLINE void
Context<Setup>::accept() {
	io_.accept();
	request_.parse(0);
}

template <typename Setup> NEXTWEB_INLINE void
Context<Setup>::finish() {
	io_.finish();
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_CONTEXT_HPP_INCLUDED
