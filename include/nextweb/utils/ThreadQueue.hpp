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

#ifndef NEXTWEB_UTILS_THREAD_QUEUE_HPP_INCLUDED
#define NEXTWEB_UTILS_THREAD_QUEUE_HPP_INCLUDED

#include <queue>
#include <functional>

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"
#include "nextweb/utils/Threads.hpp"

namespace nextweb { namespace utils {

template <typename Item, typename BoundsChecker>
class ThreadQueueBase : public AtomicShared, private BoundsChecker {

public:
	ThreadQueueBase();
	virtual ~ThreadQueueBase();
	
	bool pop(Item &item);
	void push(Item const &item);
	
	void stop();
	bool isStopped() const;

private:
	ThreadQueueBase(ThreadQueueBase const &);
	ThreadQueueBase& operator = (ThreadQueueBase const &);

	using BoundsChecker::boundsReached;

private:
	bool stopped_;
	Condition condition_;
	Mutex mutable mutex_;
	std::queue<Item> items_;
};

struct NullBoundsChecker {
	bool boundsReached(std::size_t size) const;
};

template <std::size_t N>
struct StaticBoundsChecker {
	bool boundsReached(std::size_t size) const;
	typedef StaticBoundsChecker<N> Type;
};

class DynamicBoundsChecker {

public:
	DynamicBoundsChecker();
	void setBounds(std::size_t size);
	bool boundsReached(std::size_t size) const;

private:
	std::size_t size_;
};

template <typename Item>
struct UnboundedThreadQueue : public ThreadQueueBase<Item, NullBoundsChecker> {
};

template <typename Item>
struct DynamicBoundedThreadQueue : public ThreadQueueBase<Item, DynamicBoundsChecker> {
};

template <typename Item, std::size_t N>
struct StaticBoundedThreadQueue : public ThreadQueueBase<Item, typename StaticBoundsChecker<N>::Type> {
};

template <typename Item, typename BoundsChecker> NEXTWEB_INLINE
ThreadQueueBase<Item, BoundsChecker>::ThreadQueueBase() :
	stopped_(false)
{
}

template <typename Item, typename BoundsChecker> NEXTWEB_INLINE 
ThreadQueueBase<Item, BoundsChecker>::~ThreadQueueBase() {
}

template <typename Item, typename BoundsChecker> NEXTWEB_INLINE bool
ThreadQueueBase<Item, BoundsChecker>::pop(Item &item) {
	Mutex::ScopedLock lock(mutex_);
	while (!stopped_ && items_.empty()) {
		condition_.wait(lock);
	}
	if (!stopped_) {
		Item i = items_.front();
		items_.pop();
		std::swap(i, item);
		return true;
	}
	return false;
}

template <typename Item, typename BoundsChecker> NEXTWEB_INLINE void
ThreadQueueBase<Item, BoundsChecker>::push(Item const &item) {
	Mutex::ScopedLock lock(mutex_);
	if (boundsReached(items_.size())) {
		throw Error("bounds reached in queue");
	}
	items_.push(item);
	condition_.broadcast();
}

template <typename Item, typename BoundsChecker> NEXTWEB_INLINE void
ThreadQueueBase<Item, BoundsChecker>::stop() {
	Mutex::ScopedLock lock(mutex_);
	stopped_ = true;
	condition_.broadcast();
}

template <typename Item, typename BoundsChecker> NEXTWEB_INLINE bool
ThreadQueueBase<Item, BoundsChecker>::isStopped() const {
	Mutex::ScopedLock lock(mutex_);
	return stopped_;
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/ThreadQueue.hpp"
#endif

#endif // NEXTWEB_UTILS_THREAD_QUEUE_HPP_INCLUDED
