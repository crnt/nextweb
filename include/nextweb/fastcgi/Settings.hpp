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

#ifndef NEXTWEB_FASTCGI_SETTINGS_HPP_INCLUDED
#define NEXTWEB_FASTCGI_SETTINGS_HPP_INCLUDED

#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API Settings {

public:
	Settings();
	virtual ~Settings();

private:
	Settings(Settings const &);
	Settings& operator = (Settings const &);
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_SETTINGS_HPP_INCLUDED
