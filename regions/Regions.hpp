/** @file Regions.hpp */
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

#ifndef NEXTWEB_REGIONS_REGIONS_HPP_INCOUDED
#define NEXTWEB_REGIONS_REGIONS_HPP_INCOUDED

#include "nextweb/SharedPtr.hpp"
#include "nextweb/StandardSetup.hpp"

namespace nextweb { namespace regions {

template <typename Setup>
class RegionsImpl;

class Regions {

public:
	Regions(char const *name);
	virtual ~Regions();

private:
	typedef RegionsImpl<StandardSetup> ImplType;

private:
	SharedPtr<ImplType> impl_;
};

}} // namespaces

#endif // NEXTWEB_REGIONS_REGIONS_HPP_INCOUDED