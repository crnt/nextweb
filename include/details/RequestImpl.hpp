/** @file RequestImpl.hpp */
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

#ifndef NEXTWEB_DETAILS_REQUEST_IMPL_HPP_INCLUDED
#define NEXTWEB_DETAILS_REQUEST_IMPL_HPP_INCLUDED

#include <fcgiapp.h>

#include "nextweb/SharedPtr.hpp"
#include "details/Environment.hpp"
#include "details/RequestParams.hpp"

namespace nextweb { namespace details {

class RequestImplBase
{
public:
	RequestImplBase();
	virtual ~RequestImplBase();

	bool acceptRequest();
	void finishRequest();
	void initRequest(int socket);
	Environment environ();

private:
	RequestImplBase(RequestImplBase const &);
	RequestImplBase& operator = (RequestImplBase const &);
	
private:
	bool initialized_;
	FCGX_Request request_;
};

template <typename Setup>
class RequestImpl : 
	public RequestImplBase,
	public RequestParams<Setup>,
	public Setup::ThreadingTraits::ReferenceCounted
{
public:
	RequestImpl();
	virtual ~RequestImpl();

	typedef RequestImpl<Setup> Type;
	typedef SharedPtr<Type> PtrType;

	void finish();
	bool accept(std::size_t threshold);

private:
	RequestImpl(RequestImpl const &);
	RequestImpl& operator = (RequestImpl const &);
};

template <typename Setup> inline 
RequestImpl<Setup>::RequestImpl()
{
}

template <typename Setup> inline
RequestImpl<Setup>::~RequestImpl()
{
}

template <typename Setup> inline void
RequestImpl<Setup>::finish() {
	finishRequest();
}

template <typename Setup> inline bool
RequestImpl<Setup>::accept(std::size_t threshold) {
	if (acceptRequest()) {
		parse(environ(), threshold);
	}
	return false;
}

}} // namespaces

#endif // NEXTWEB_DETAILS_REQUEST_IMPL_HPP_INCLUDED
