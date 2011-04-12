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

#ifndef NEXTWEB_INLINES_UTILS_THREAD_QUEUE_HPP_INCLUDED
#define NEXTWEB_INLINES_UTILS_THREAD_QUEUE_HPP_INCLUDED

namespace nextweb { namespace utils {

NEXTWEB_INLINE bool
NullBoundsChecker::boundsReached(std::size_t size) const {
	(void) size;
	return false;
}

template <std::size_t N> NEXTWEB_INLINE bool
StaticBoundsChecker<N>::boundsReached(std::size_t size) const {
	return N == size;
}

DynamicBoundsChecker::DynamicBoundsChecker() :
	size_(0)
{
}

NEXTWEB_INLINE void
DynamicBoundsChecker::setBounds(std::size_t size) {
	size_ = size;
}

NEXTWEB_INLINE bool
DynamicBoundsChecker::boundsReached(std::size_t size) const {
	if (0 != size_) {
		return size_ == size;
	}
	throw Error("thread queue was not initialized properly");
}

}} // namespaces

#endif // NEXTWEB_INLINES_UTILS_THREAD_QUEUE_HPP_INCLUDED
