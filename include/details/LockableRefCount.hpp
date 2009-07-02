/** @file LockableRefCount.hpp */
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

#ifndef NEXTWEB_DETAILS_LOCKABLE_REF_COUNT_HPP_INCLUDED
#define NEXTWEB_DETAILS_LOCKABLE_REF_COUNT_HPP_INCLUDED

#include <cassert>

#include "details/Mutex.hpp"

namespace nextweb { namespace details {

class LockableRefCount;

void incRef(LockableRefCount *object);
void decRef(LockableRefCount *object);

class LockableRefCount {

public:
	LockableRefCount();
	virtual ~LockableRefCount();

private:
	LockableRefCount(LockableRefCount const &);
	LockableRefCount& operator = (LockableRefCount const &);

	friend void incRef(LockableRefCount *object);
	friend void decRef(LockableRefCount *object);

private:
	int count_;
	Mutex mutex_;
};

inline
LockableRefCount::LockableRefCount() :
	count_(0)
{
}

inline
LockableRefCount::~LockableRefCount() {
	assert(0 == count_);
}

inline void
incRef(LockableRefCount *object) {
	Mutex::Lock lock(object->mutex_);
	++object->count_;
}

inline void
decRef(LockableRefCount *object) {
	Mutex::Lock lock(object->mutex_);
	if (0 == --object->count_) {
		lock.unlock();
		delete object;
	}
}

}} // namespaces

#endif // NEXTWEB_DETAILS_LOCKABLE_REF_COUNT_HPP_INCLUDED
