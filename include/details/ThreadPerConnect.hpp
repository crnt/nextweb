/** @file ThreadPerConnect.hpp */
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

#ifndef NEXTWEB_DETAILS_THREAD_PER_CONNECT_HPP_INCLUDED
#define NEXTWEB_DETAILS_THREAD_PER_CONNECT_HPP_INCLUDED

#include <cstdio>

#include "details/Mutex.hpp"
#include "details/AcceptTask.hpp"
#include "details/ThreadGroup.hpp"
#include "details/RequestImpl.hpp"

#include "nextweb/Error.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/RuntimeSettings.hpp"

namespace nextweb { namespace details {

template <typename Setup>
class ThreadPerConnect : private ThreadGroup {

public:
	ThreadPerConnect();
	virtual ~ThreadPerConnect();
	
	typedef ThreadPerConnect<Setup> Type;
	typedef ThreadPerConnectSettings SettingsType;
	
	virtual void finishRequests();
	virtual void handleRequests(RuntimeSettings const &settings);
	
private:
	ThreadPerConnect(ThreadPerConnect const &);
	ThreadPerConnect& operator = (ThreadPerConnect const &);
	
	friend class AcceptTask<Type>;
	typedef RequestImpl<Setup> RequestType;

	void finishAcceptLoop();
	void acceptLoop(ThreadPerConnectSettings const &settings);
	bool isAcceptLoopFinished() const;

private:
	int socket_;
	bool loopFinished_;
	mutable Mutex mutex_;
	std::size_t threadCount_;
};

template <typename Setup> inline 
ThreadPerConnect<Setup>::ThreadPerConnect() :
	socket_(0), loopFinished_(false), threadCount_(0)
{
}

template <typename Setup> inline 
ThreadPerConnect<Setup>::~ThreadPerConnect() {
}

template <typename Setup> inline void
ThreadPerConnect<Setup>::finishRequests() {
	finishAcceptLoop();
	join();
}

template <typename Setup> inline void
ThreadPerConnect<Setup>::handleRequests(RuntimeSettings const &settings) {
	
	ThreadPerConnectSettings const &tps = dynamic_cast<ThreadPerConnectSettings const&>(settings);
	try {
		std::size_t threadCount = tps.getThreadCount();
		start(threadCount, AcceptTask<Type>(this, tps));
	}
	catch (Error const &e) {
		finishRequests();
		throw;
	}
}

template <typename Setup> inline void
ThreadPerConnect<Setup>::finishAcceptLoop() {
	Mutex::Lock lock(mutex_);
	loopFinished_ = true;
}

template <typename Setup> inline void
ThreadPerConnect<Setup>::acceptLoop(ThreadPerConnectSettings const &settings) {
	
	off_t threshold = settings.getPostSizeThreshold();
	typename RequestImpl<Setup>::PtrType request(new RequestImpl<Setup>());
	
	request->initRequest(settings.getSocket());
	while (!isAcceptLoopFinished() && request->accept(threshold)) {
		try {
			// TODO handle request
			request->finishRequest();
		}
		catch (Error const &e) {
		}
	}
	finishRequests();
}

template <typename Setup> inline bool
ThreadPerConnect<Setup>::isAcceptLoopFinished() const {
	Mutex::Lock lock(mutex_);
	return loopFinished_;
}

}} // namespaces

#endif // NEXTWEB_DETAILS_THREAD_PER_CONNECT_HPP_INCLUDED
