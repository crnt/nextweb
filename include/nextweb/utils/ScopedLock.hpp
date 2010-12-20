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

#ifndef NEXTWEB_UTILS_SCOPED_LOCK_HPP_INCLUDED
#define NEXTWEB_UTILS_SCOPED_LOCK_HPP_INCLUDED

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"

namespace nextweb { namespace utils {

class LockError : public Error {

public:
	LockError(char const *message);
};

template <typename LockableObject>
class ScopedLock {

public:
	ScopedLock(LockableObject &l, bool initiallyLocked = false);
	virtual ~ScopedLock();

	void lock();
	void unlock();
	LockableObject& object();
	
	bool locked() const;
	class BoolConvertible;
	operator BoolConvertible const* () const;

private:
	ScopedLock(ScopedLock const &);
	ScopedLock& operator = (ScopedLock const &);
	
	void makeLock(LockableObject &object);
	void makeUnlock(LockableObject &object);

private:
	bool locked_;
	LockableObject &object_;
};

template <typename LockableObject> NEXTWEB_INLINE
ScopedLock<LockableObject>::ScopedLock(LockableObject &l, bool initiallyLocked) :
	locked_(initiallyLocked), object_(l)
{
	if (!locked()) lock();
}

template <typename LockableObject> NEXTWEB_INLINE
ScopedLock<LockableObject>::~ScopedLock()
{
	if (locked()) unlock();
}

template <typename LockableObject> NEXTWEB_INLINE void
ScopedLock<LockableObject>::lock() {
	if (locked()) {
		throw LockError("already locked");
	}
	makeLock(object_);
	locked_ = true;
}

template <typename LockableObject> NEXTWEB_INLINE void
ScopedLock<LockableObject>::unlock() {
	if (!locked()) {
		throw LockError("not locked");
	}
	makeUnlock(object_);
	locked_ = false;
}

template <typename LockableObject> NEXTWEB_INLINE bool
ScopedLock<LockableObject>::locked() const {
	return locked_;
}

template <typename LockableObject> NEXTWEB_INLINE 
ScopedLock<LockableObject>::operator typename ScopedLock<LockableObject>::BoolConvertible const* () const {
	return locked() ? reinterpret_cast<BoolConvertible const*>(this) : 0;
}

template <typename LockableObject> NEXTWEB_INLINE LockableObject&
ScopedLock<LockableObject>::object() {
	return object_;
}

template <typename LockableObject> NEXTWEB_INLINE void
ScopedLock<LockableObject>::makeLock(LockableObject &object) {
	object.lock();
}

template <typename LockableObject> NEXTWEB_INLINE void
ScopedLock<LockableObject>::makeUnlock(LockableObject &object) {
	object.unlock();
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/ScopedLock.hpp"
#endif

#endif // NEXTWEB_UTILS_SCOPED_LOCK_HPP_INCLUDED
