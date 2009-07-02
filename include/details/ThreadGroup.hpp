/** @file ThreadGroup.hpp */
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

#ifndef NEXTWEB_DETAILS_THREAD_GROUP_HPP_INCLUDED
#define NEXTWEB_DETAILS_THREAD_GROUP_HPP_INCLUDED

#include <list>

#include "details/Thread.hpp"
#include "nextweb/SharedPtr.hpp"

namespace nextweb { namespace details {

class ThreadGroup {

public:
	ThreadGroup();
	virtual ~ThreadGroup();

	void join();
	template <typename Task> void start(std::size_t size, Task const &task);

private:
	ThreadGroup(ThreadGroup const &);
	ThreadGroup& operator = (ThreadGroup const &);
	typedef SharedPtr<Thread> ThreadPtr;

private:
	std::list<ThreadPtr> threads_;
};

template <typename Task> inline void
ThreadGroup::start(std::size_t size, Task const &task) {
	for (std::size_t i = 0; i < size; ++i) {
		ThreadPtr thread = TaskThread<Task>::create(task);
		threads_.push_back(thread);
	}
	for (std::list<ThreadPtr>::iterator i = threads_.begin(), end = threads_.end(); i != end; ++i) {
		(*i)->start();
	}
}

}} // namespaces

#endif // NEXTWEB_DETAILS_THREAD_GROUP_HPP_INCLUDED
