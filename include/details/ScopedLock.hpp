/** @file ScopedLock.hpp */
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

#ifndef NEXTWEB_DETAILS_SCOPED_LOCK_HPP_INCLUDED
#define NEXTWEB_DETAILS_SCOPED_LOCK_HPP_INCLUDED

#include "nextweb/Error.hpp"

namespace nextweb { namespace details {

template <typename Lock>
class ScopedLock {

public:
	ScopedLock(Lock &l, bool initiallyLocked = false);
	virtual ~ScopedLock();

	void lock();
	void unlock();
	bool locked() const;

	Lock& lockedObject();
	
	class BoolConvertible;
	operator BoolConvertible const* () const;

private:
	ScopedLock(ScopedLock const &);
	ScopedLock& operator = (ScopedLock const &);

	void makeLock(Lock &lock);
	void makeUnlock(Lock &lock);

private:
	Lock &lock_;
	bool locked_;
};

class LockError : public Error {

public:
	LockError(char const *message);
	virtual ~LockError() throw ();
};

template <typename Lock> inline
ScopedLock<Lock>::ScopedLock(Lock &l, bool initiallyLocked) :
	lock_(l), locked_(initiallyLocked)
{
	if (!locked()) lock();
}

template <typename Lock> inline
ScopedLock<Lock>::~ScopedLock()
{
	
	if (locked()) unlock();
}

template <typename Lock> inline void
ScopedLock<Lock>::lock() {
	if (locked()) {
		throw LockError("already locked");
	}
	makeLock(lock_);
	locked_ = true;
}

template <typename Lock> inline void
ScopedLock<Lock>::unlock() {
	if (!locked()) {
		throw LockError("not locked");
	}
	makeUnlock(lock_);
	locked_ = false;
}

template <typename Lock> inline bool
ScopedLock<Lock>::locked() const {
	return locked_;
}

template <typename Lock> inline Lock&
ScopedLock<Lock>::lockedObject() {
	return lock_;
}

template <typename Lock> inline 
ScopedLock<Lock>::operator typename ScopedLock<Lock>::BoolConvertible const* () const {
	return locked() ? reinterpret_cast<BoolConvertible const*>(this) : 0;
}

template <typename Lock> inline void
ScopedLock<Lock>::makeLock(Lock &lock) {
	lock.lock();
}

template <typename Lock> inline void
ScopedLock<Lock>::makeUnlock(Lock &lock) {
	lock.unlock();
}

struct SingleThreadedMutex;

template <> inline void
ScopedLock<SingleThreadedMutex>::makeLock(SingleThreadedMutex &) {
}

template <> inline void
ScopedLock<SingleThreadedMutex>::makeUnlock(SingleThreadedMutex &) {
}

}} // namespaces

#endif // NEXTWEB_DETAILS_SCOPED_LOCK_HPP_INCLUDED
