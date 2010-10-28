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

#ifndef NEXTWEB_UTILS_THREADS_HPP_INCLUDED
#define NEXTWEB_UTILS_THREADS_HPP_INCLUDED

#include <list>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/utils/PosixThreads.hpp"

namespace nextweb { namespace utils {

class LockableShared;

void incRef(LockableShared *object);
void decRef(LockableShared *object);

class LockableShared {

public:
	LockableShared();
	virtual ~LockableShared();

private:
	LockableShared(LockableShared const &);
	LockableShared& operator = (LockableShared const &);

	friend void incRef(LockableShared *object);
	friend void decRef(LockableShared *object);

private:
	int count_;
	Mutex mutex_;
};

class SharedThread : public Thread, public LockableShared {

public:
	SharedThread();
	virtual ~SharedThread();

private:
	SharedThread(SharedThread const &);
	SharedThread& operator = (SharedThread const &);
};

template <typename Task>
class TaskThread : public SharedThread {

public:
	TaskThread(Task const &task);
	virtual ~TaskThread();
	virtual void run();
	
private:
	TaskThread(TaskThread const &);
	TaskThread& operator = (TaskThread const &);
	
private:
	Task task_;
};

class ThreadGroup {

public:
	ThreadGroup();
	virtual ~ThreadGroup();

	void joinAll();
	template <typename Task> void start(std::size_t size, Task const &task);

private:
	ThreadGroup(ThreadGroup const &);
	ThreadGroup& operator = (ThreadGroup const &);
	typedef SharedPtr<SharedThread> ThreadPtr;

private:
	std::list<ThreadPtr> threads_;
};

template <typename Task> NEXTWEB_INLINE
TaskThread<Task>::TaskThread(Task const &task) :
	task_(task)
{
}

template <typename Task> NEXTWEB_INLINE
TaskThread<Task>::~TaskThread() {
}

template <typename Task> NEXTWEB_INLINE void
TaskThread<Task>::run() {
	task_();
}

template <typename Task> NEXTWEB_INLINE void
ThreadGroup::start(std::size_t size, Task const &task) {
	for (std::size_t i = 0; i < size; ++i) {
		ThreadPtr thread(new TaskThread<Task>(task));
		threads_.push_back(thread);
	}
	for (std::list<ThreadPtr>::iterator i = threads_.begin(), end = threads_.end(); i != end; ++i) {
		(*i)->start();
	}
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/Threads.hpp"
#endif

#endif // NEXTWEB_UTILS_THREADS_HPP_INCLUDED
