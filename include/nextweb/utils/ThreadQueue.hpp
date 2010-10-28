// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/utils/Threads.hpp"

namespace nextweb { namespace utils {

template <typename Item>
class ThreadQueue : public LockableShared {

public:
	ThreadQueue();
	virtual ~ThreadQueue();
	
	typedef ThreadQueue<Item> Type;
	typedef SharedPtr<Type> Pointer;

	bool pop(Item &item);
	void push(Item const &item);
	
	void stop();
	bool isStopped() const;

private:
	ThreadQueue(ThreadQueue const &);
	ThreadQueue& operator = (ThreadQueue const &);

private:
	bool stopped_;
	Condition condition_;
	mutable Mutex mutex_;
	std::queue<Item> items_;
};

template <typename Item> NEXTWEB_INLINE
ThreadQueue<Item>::ThreadQueue() :
	stopped_(false)
{
}

template <typename Item> NEXTWEB_INLINE 
ThreadQueue<Item>::~ThreadQueue() {
}

template <typename Item> NEXTWEB_INLINE bool
ThreadQueue<Item>::pop(Item &item) {
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

template <typename Item> NEXTWEB_INLINE void
ThreadQueue<Item>::push(Item const &item) {
	Mutex::ScopedLock lock(mutex_);
	items_.push(item);
	condition_.broadcast();
}

template <typename Item> NEXTWEB_INLINE void
ThreadQueue<Item>::stop() {
	Mutex::ScopedLock lock(mutex_);
	stopped_ = true;
	condition_.broadcast();
}

template <typename Item> NEXTWEB_INLINE bool
ThreadQueue<Item>::isStopped() const {
	Mutex::ScopedLock lock(mutex_);
	return stopped_;
}

}} // namespaces

#endif // NEXTWEB_UTILS_THREAD_QUEUE_HPP_INCLUDED
