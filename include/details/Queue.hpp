/** @file Queue.hpp */
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

#ifndef NEXTWEB_DETAILS_QUEUE_HPP_INCLUDED
#define NEXTWEB_DETAILS_QUEUE_HPP_INCLUDED

#include <queue>
#include <functional>

#include "details/Mutex.hpp"
#include "details/Condition.hpp"
#include "details/LockableRefCount.hpp"
#include "nextweb/SharedPtr.hpp"

namespace nextweb { namespace details {

template <typename Item>
class Queue : public LockableRefCount {

public:
	Queue();
	virtual ~Queue();
	
	typedef Queue<Item> Type;
	typedef SharedPtr<Type> PtrType;

	bool pop(Item &item);
	void push(Item const &item);
	void finish();

private:
	Queue(Queue const &);
	Queue& operator = (Queue const &);

private:
	Mutex mutex_;
	bool finished_;
	Condition condition_;
	std::queue<Item> items_;
};

template <typename Item> inline
Queue<Item>::Queue() :
	finished_(false)
{
}

template <typename Item> inline 
Queue<Item>::~Queue() {
}

template <typename Item> inline bool
Queue<Item>::pop(Item &item) {
	Mutex::Lock lock(mutex_);
	while (!finished_ && items_.empty()) {
		condition_.wait(lock);
	}
	if (!finished_) {
		Item i = items_.front();
		items_.pop();
		std::swap(i, item);
		return true;
	}
	return false;
}

template <typename Item> inline void
Queue<Item>::push(Item const &item) {
	Mutex::Lock lock(mutex_);
	items_.push(item);
	condition_.broadcast();
}

template <typename Item> inline void
Queue<Item>::finish() {
	Mutex::Lock lock(mutex_);
	finished_ = true;
	condition_.broadcast();
}

}} // namespaces

#endif // NEXTWEB_DETAILS_QUEUE_HPP_INCLUDED
