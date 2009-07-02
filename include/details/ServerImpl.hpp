/** @file ServerImpl.hpp */
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

#ifndef NEXTWEB_DETAILS_SERVER_IMPL_HPP_INCLUDED
#define NEXTWEB_DETAILS_SERVER_IMPL_HPP_INCLUDED

#include "nextweb/RuntimeSettings.hpp"

namespace nextweb { namespace details {

template <typename Setup>
class ServerImpl : public Setup::ThreadingTraits::ReferenceCounted {

public:
	ServerImpl();
	virtual ~ServerImpl();

	virtual void finishRequests() = 0;
	virtual void handleRequests(RuntimeSettings const &settings) = 0;

private:
	ServerImpl(ServerImpl const &);
	ServerImpl& operator = (ServerImpl const &);
};

template <typename ThreadingModel, typename Setup>
class ConcreteServerImpl : public ServerImpl<Setup>, public ThreadingModel {

public:
	ConcreteServerImpl();
	virtual ~ConcreteServerImpl();

	virtual void finishRequests();
	virtual void handleRequests(RuntimeSettings const &settings);

private:
	ConcreteServerImpl(ConcreteServerImpl const &);
	ConcreteServerImpl& operator = (ConcreteServerImpl const &);
};

template <typename Setup> inline 
ServerImpl<Setup>::ServerImpl()
{
}

template <typename Setup> inline 
ServerImpl<Setup>::~ServerImpl() {
}

template <typename ThreadingModel, typename Setup> inline 
ConcreteServerImpl<ThreadingModel, Setup>::ConcreteServerImpl()
{
}

template <typename ThreadingModel, typename Setup> inline 
ConcreteServerImpl<ThreadingModel, Setup>::~ConcreteServerImpl() {
}

template <typename ThreadingModel, typename Setup> inline void
ConcreteServerImpl<ThreadingModel, Setup>::finishRequests() {
	ThreadingModel::finishRequests();
}

template <typename ThreadingModel, typename Setup> inline void
ConcreteServerImpl<ThreadingModel, Setup>::handleRequests(RuntimeSettings const &settings) {
	ThreadingModel::handleRequests(settings);
}


}} // namespaces

#endif // NEXTWEB_DETAILS_SERVER_IMPL_HPP_INCLUDED
