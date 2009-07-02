/** @file ReferenceCounted.hpp */
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

#ifndef NEXTWEB_REFERENCE_COUNTED_HPP_INCLUDED
#define NEXTWEB_REFERENCE_COUNTED_HPP_INCLUDED

#include <cassert>

#include "nextweb/Config.hpp"

namespace nextweb {

class ReferenceCounted;

void incRef(ReferenceCounted *object);
void decRef(ReferenceCounted *object);

class NEXTWEB_API ReferenceCounted {

public:
	ReferenceCounted();
	virtual ~ReferenceCounted();

private:
	ReferenceCounted(ReferenceCounted const &);
	ReferenceCounted& operator = (ReferenceCounted const &);

	friend void incRef(ReferenceCounted *object);
	friend void decRef(ReferenceCounted *object);

private:
	int count_;
};

inline
ReferenceCounted::ReferenceCounted() :
	count_(0)
{
}

inline
ReferenceCounted::~ReferenceCounted() {
	assert(0 == count_);
}

NEXTWEB_API inline void
incRef(ReferenceCounted *object) {
	++object->count_;
}

NEXTWEB_API inline void
decRef(ReferenceCounted *object) {
	if (0 == --object->count_) delete object;
}

} // namespace

#endif // NEXTWEB_REFERENCE_COUNTED_HPP_INCLUDED
