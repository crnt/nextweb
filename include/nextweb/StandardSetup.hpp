/** @file StandardSetup.hpp */
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

#ifndef NEXTWEB_STANDARD_SETUP_HPP_INCLUDED
#define NEXTWEB_STANDARD_SETUP_HPP_INCLUDED

#include <new>
#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/DetailsForward.hpp"

namespace nextweb {

struct NEXTWEB_API StandardSetup {

	typedef std::string StringType;
	
	typedef details::MultiThreaded ThreadingTraits;
	
	template <typename T> struct Allocator {
		typedef std::allocator<T> Type;
	};
};

} // namespace

#endif // NEXTWEB_STANDARD_SETUP_HPP_INCLUDED
