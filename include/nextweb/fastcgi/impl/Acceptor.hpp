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

#ifndef NEXTWEB_FASTCGI_ACCEPTOR_HPP_INCLUDED
#define NEXTWEB_FASTCGI_ACCEPTOR_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/utils/Threads.hpp"
#include "nextweb/fastcgi/impl/HandlerData.hpp"

namespace nextweb { namespace fastcgi {

template <typename Setup>
class Acceptor;

template <typename Setup>
class AcceptorTask {

public:
	AcceptorTask(Acceptor<Setup> *acceptor, int socket);
	void operator () ();

private:
	int socket_;
	Acceptor<Setup> *acceptor_;
};

template <typename Setup>
class Acceptor : public Setup::DispatchType, private utils::ThreadGroup {

public:
	Acceptor();
	virtual ~Acceptor();

	typedef HandlerData<Setup> HandlerDataType;
	typedef typename Setup::DispatchType BaseType;
	
	void stop();
	void start(int socket, std::size_t nthreads);

private:
	Acceptor(Acceptor const &);
	Acceptor& operator = (Acceptor const &);
	
	void run(int socket);
	friend class AcceptorTask<Setup>;
};

template <typename Setup> NEXTWEB_INLINE
AcceptorTask<Setup>::AcceptorTask(Acceptor<Setup> *acceptor, int socket) :
	socket_(socket), acceptor_(acceptor)
{
}

template <typename Setup> NEXTWEB_INLINE void
AcceptorTask<Setup>::operator () () {
	acceptor_->run(socket_);
}

template <typename Setup> NEXTWEB_INLINE
Acceptor<Setup>::Acceptor()
{
}

template <typename Setup> NEXTWEB_INLINE
Acceptor<Setup>::~Acceptor() {
}

template <typename Setup> NEXTWEB_INLINE void
Acceptor<Setup>::stop() {
	ThreadGroup::joinAll();
}

template <typename Setup> NEXTWEB_INLINE void
Acceptor<Setup>::start(int socket, std::size_t nthreads) {
	AcceptorTask<Setup> task(this, socket);
	ThreadGroup::start(nthreads, task);
}

template <typename Setup> NEXTWEB_INLINE void
Acceptor<Setup>::run(int socket) {
	SharedPtr<HandlerDataType> data(new HandlerDataType(socket));
	while (data->accept()) {
		
		data->finish();
	}
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_ACCEPTOR_HPP_INCLUDED
