/** @file Condition.hpp */
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

#ifndef NEXTWEB_DETAILS_CONDITION_HPP_INCLUDED
#define NEXTWEB_DETAILS_CONDITION_HPP_INCLUDED

#include <pthread.h>

#include "details/Mutex.hpp"

namespace nextweb { namespace details {

class TimeSpec;

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

template <typename Lock> void
Condition::wait(Lock &lock) {
	MutexBase &mutex = static_cast<MutexBase&>(lock.lockedObject());
	wait(mutex.impl_);
}

template <typename Lock> bool
Condition::timedWait(Lock &lock, TimeSpec const &ts) {
	MutexBase &mutex = static_cast<MutexBase&>(lock.lockedObject());
	return timedWait(mutex.impl_, ts);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_CONDITION_HPP_INCLUDED
