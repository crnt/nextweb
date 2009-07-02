/** @file Thread.hpp */
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

#ifndef NEXTWEB_DETAILS_THREAD_HPP_INCLUDED
#define NEXTWEB_DETAILS_THREAD_HPP_INCLUDED

#include <pthread.h>

#include "nextweb/SharedPtr.hpp"
#include "details/MultiThreaded.hpp"

namespace nextweb { namespace details {

class Thread : public MultiThreaded::ReferenceCounted {

public:
	void join();
	void start();
	
protected:
	Thread();
	virtual ~Thread();
	virtual void run() = 0;

private:
	Thread(Thread const &);
	Thread& operator = (Thread const &);
	static void* threadFunction(void *thread);

private:
	pthread_t impl_;
};

template <typename Task>
class TaskThread : public Thread {

public:
	static SharedPtr<Thread> create(Task const &task);
	
protected:
	TaskThread(Task const &task);
	virtual ~TaskThread();
	virtual void run();
	
private:
	TaskThread(TaskThread const &);
	TaskThread& operator = (TaskThread const &);
	
private:
	Task task_;
};

template <typename Task> inline
TaskThread<Task>::TaskThread(Task const &task) :
	task_(task)
{
}

template <typename Task> inline
TaskThread<Task>::~TaskThread() {
}

template <typename Task> inline void
TaskThread<Task>::run() {
	task_();
}

template <typename Task> inline SharedPtr<Thread>
TaskThread<Task>::create(Task const &task) {
	return SharedPtr<Thread>(new TaskThread(task));
}

}} // namespaces 

#endif // NEXTWEB_DETAILS_THREAD_HPP_INCLUDED
