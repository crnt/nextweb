/** @file ThreadPool.hpp */
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

#ifndef NEXTWEB_DETAILS_THREAD_POOL_HPP_INCLUDED
#define NEXTWEB_DETAILS_THREAD_POOL_HPP_INCLUDED

#include <map>
#include <cstdio>
#include <string>

#include "details/Mutex.hpp"
#include "details/Queue.hpp"
#include "details/AcceptTask.hpp"
#include "details/RequestImpl.hpp"
#include "details/ThreadGroup.hpp"

#include "nextweb/Error.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/RuntimeSettings.hpp"

namespace nextweb { namespace details {

template <typename Setup>
class HandleTask;

template <typename Setup>
class ThreadPool : private ThreadGroup {

public:
	ThreadPool();
	virtual ~ThreadPool();
	
	typedef ThreadPool<Setup> Type;
	typedef ThreadPoolSettings SettingsType;

	virtual void finishRequests();
	virtual void handleRequests(RuntimeSettings const &settings);

private:
	ThreadPool(ThreadPool const &);
	ThreadPool& operator = (ThreadPool const &);
	
	friend class AcceptTask<Type>;
	friend class HandleTask<Setup>;
	
	typedef typename RequestImpl<Setup>::PtrType RequestPtrType;
	typedef typename Queue<RequestPtrType>::PtrType QueuePtrType;

	void finishAcceptLoop();
	void acceptLoop(ThreadPoolSettings const &settings);
	bool isAcceptLoopFinished() const;
	
	void handleLoop(ThreadPoolSettings const &settings, QueuePtrType queue);

private:
	bool loopFinished_;
	mutable Mutex mutex_;
	std::map<std::string, QueuePtrType> queues_;
};

template <typename Setup>
class HandleTask {

public:
	HandleTask(ThreadPool<Setup> *object, ThreadPoolSettings const &tps, typename ThreadPool<Setup>::QueuePtrType const &queue);
	void operator () ();
	
private:
	ThreadPool<Setup> *object_;
	ThreadPoolSettings const &settings_;
	typename ThreadPool<Setup>::QueuePtrType queue_;
};

template <typename Setup> inline
ThreadPool<Setup>::ThreadPool() :
	loopFinished_(false)
{
}

template <typename Setup> inline 
ThreadPool<Setup>::~ThreadPool() {
}

template <typename Setup> inline void
ThreadPool<Setup>::finishRequests() {
	finishAcceptLoop();
	for (typename std::map<std::string, QueuePtrType>::iterator i = queues_.begin(), end = queues_.end(); i != end; ++i) {
		i->second->finish();
	}
	join();
}

template <typename Setup> inline void
ThreadPool<Setup>::handleRequests(RuntimeSettings const &settings) {
	ThreadPoolSettings const &tps = dynamic_cast<ThreadPoolSettings const&>(settings);
	try {
		start(tps.getAcceptorCount(), AcceptTask<Type>(this, tps));
		Enumeration<QueueParams>::PtrType queueParams = tps.getQueueParams();
		while (queueParams->hasMoreElements()) {
			QueueParams params = queueParams->nextElement();
			QueuePtrType queue(new Queue<RequestPtrType>());
			queues_.insert(std::make_pair(params.name, queue));
			start(params.threadCount, HandleTask<Setup>(this, tps, queue));
		}
	}
	catch (Error const &e) {
		finishRequests();
		throw;
	}
}

template <typename Setup> inline void
ThreadPool<Setup>::finishAcceptLoop() {
	Mutex::Lock lock(mutex_);
	loopFinished_ = true;
}

template <typename Setup> inline void
ThreadPool<Setup>::acceptLoop(ThreadPoolSettings const &settings) {
	
	off_t threshold = settings.getPostSizeThreshold();
	while (!isAcceptLoopFinished()) {
		typename RequestImpl<Setup>::PtrType request(new RequestImpl<Setup>());
		request->initRequest(settings.getSocket());
		if (!request->accept(threshold)) {
			// TODO find appropriate queue and put the request into it
			break;
		}
	}
}

template <typename Setup> inline bool
ThreadPool<Setup>::isAcceptLoopFinished() const {
	Mutex::Lock lock(mutex_);
	return loopFinished_;
}

template <typename Setup> inline void
ThreadPool<Setup>::handleLoop(ThreadPoolSettings const &settings, typename ThreadPool<Setup>::QueuePtrType queue) {
	typename RequestImpl<Setup>::PtrType request;
	while (queue->pop(request)) {
		// TODO handleRequest
		request->finish();
	}
}

template <typename Setup> inline 
HandleTask<Setup>::HandleTask(ThreadPool<Setup> *object, ThreadPoolSettings const &tps, typename ThreadPool<Setup>::QueuePtrType const &queue) :
	object_(object), settings_(tps), queue_(queue)
{
}

template <typename Setup> inline void
HandleTask<Setup>::operator () () {
	object_->handleLoop(settings_, queue_);
}


}} // namespaces

#endif // NEXTWEB_DETAILS_THREAD_POOL_HPP_INCLUDED
