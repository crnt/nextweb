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

#ifndef NEXTWEB_FASTCGI_DEFAULT_INVOKER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_DEFAULT_INVOKER_HPP_INCLUDED

#include <cassert>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/fastcgi/impl/Context.hpp"

namespace nextweb { namespace fastcgi {

template <typename Setup>
class DefaultInvoker {

public:
	DefaultInvoker();
	virtual ~DefaultInvoker();

	typedef Context<Setup> ContextType;
	typedef typename Setup::HandlerType HandlerType;

	void handle(SharedPtr<ContextType> const &ctx);
	void setHandler(SharedPtr<HandlerType> const &handler);
	
private:
	DefaultInvoker(DefaultInvoker const &);
	DefaultInvoker& operator = (DefaultInvoker const &);
	
private:
	SharedPtr<HandlerType> handler_;
};

template <typename Setup> NEXTWEB_INLINE 
DefaultInvoker<Setup>::DefaultInvoker() :
	handler_()
{
}

template <typename Setup> NEXTWEB_INLINE
DefaultInvoker<Setup>::~DefaultInvoker() {
}

template <typename Setup> NEXTWEB_INLINE void
DefaultInvoker<Setup>::handle(SharedPtr<typename DefaultInvoker<Setup>::ContextType> const &ctx) {
	try {
	}
	catch (Error const &e) {
	}
}

template <typename Setup> NEXTWEB_INLINE void
DefaultInvoker<Setup>::setHandler(SharedPtr<typename DefaultInvoker<Setup>::HandlerType> const &handler) {
	assert(handler);
	handler_ = handler;
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_DEFAULT_INVOKER_HPP_INCLUDED
