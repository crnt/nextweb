// nextweb - modern web framework for Python and C++
// Copyright (C) 2011 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef NEXTWEB_INLINES_UTILS_POSIX_THREADS_HPP_INCLUDED
#define NEXTWEB_INLINES_UTILS_POSIX_THREADS_HPP_INCLUDED

#include <cassert>

namespace nextweb { namespace utils {

NEXTWEB_INLINE
AtomicShared::AtomicShared() :
	count_(0)
{
}

NEXTWEB_INLINE
AtomicShared::~AtomicShared() {
	assert(0 == count_);
}

NEXTWEB_INLINE void
incRef(AtomicShared *object) {
	Mutex::ScopedLock lock(object->mutex_);
	++object->count_;
}

NEXTWEB_INLINE void
decRef(AtomicShared *object) {
	Mutex::ScopedLock lock(object->mutex_);
	if (0 == --object->count_) {
		lock.unlock();
		delete object;
	}
}

}} // namespaces

#endif // NEXTWEB_INLINES_UTILS_POSIX_THREADS_HPP_INCLUDED
