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

#ifndef NEXTWEB_UTILS_POSIX_THREADS_HPP_INCLUDED
#define NEXTWEB_UTILS_POSIX_THREADS_HPP_INCLUDED

#include <pthread.h>

#include "nextweb/Config.hpp"
#include "nextweb/utils/ScopedLock.hpp"

namespace nextweb { namespace utils {

class TimeSpec;

class Thread {

public:
	Thread();
	virtual ~Thread();

	void join();
	void start();
	virtual void run() = 0;
	
private:
	Thread(Thread const &);
	Thread& operator = (Thread const &);
	static void* threadFunction(void *thread);

private:
	pthread_t impl_;
};

class Condition {

public:
	Condition();
	virtual ~Condition();

	void signal();
	void broadcast();
	
	template <typename Lock> void wait(Lock &lock);
	template <typename Lock> bool timedWait(Lock &lock, TimeSpec const &ts);

private:
	Condition(Condition const &);
	Condition& operator = (Condition const &);

	void wait(pthread_mutex_t &mutex);
	bool timedWait(pthread_mutex_t &mutex, TimeSpec const &ts);

private:
	pthread_cond_t impl_;
};

class MutexBase {

protected:
	MutexBase(int type);
	virtual ~MutexBase();

	void lock();
	void unlock();
	bool trylock();
	
private:
	MutexBase(MutexBase const &);
	MutexBase& operator = (MutexBase const &);
	friend class Condition;

private:
	pthread_mutex_t impl_;
};

template <int Mode>
class ConcreteMutex : public MutexBase {

public:
	ConcreteMutex();
	virtual ~ConcreteMutex();
	
	typedef ConcreteMutex<Mode> Type;
	typedef nextweb::utils::ScopedLock<Type> ScopedLock;

private:
	ConcreteMutex(ConcreteMutex const &);
	ConcreteMutex& operator = (ConcreteMutex const &);
	friend class nextweb::utils::ScopedLock<Type>;
};

typedef ConcreteMutex<PTHREAD_MUTEX_NORMAL> Mutex;
typedef ConcreteMutex<PTHREAD_MUTEX_RECURSIVE> RecursiveMutex;

template <typename Lock> void
Condition::wait(Lock &lock) {
	MutexBase &mutex = static_cast<MutexBase&>(lock.object());
	wait(mutex.impl_);
}

template <typename Lock> bool
Condition::timedWait(Lock &lock, TimeSpec const &ts) {
	MutexBase &mutex = static_cast<MutexBase&>(lock.lockedObject());
	return timedWait(mutex.impl_, ts);
}

template <int Mode> NEXTWEB_INLINE 
ConcreteMutex<Mode>::ConcreteMutex() :
	MutexBase(Mode)
{
}

template <int Mode> NEXTWEB_INLINE
ConcreteMutex<Mode>::~ConcreteMutex() {
}

}} // namespaces

#endif // NEXTWEB_UTILS_POSIX_THREADS_HPP_INCLUDED
