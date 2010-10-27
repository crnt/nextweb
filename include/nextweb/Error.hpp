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

#ifndef NEXTWEB_ERROR_HPP_INCLUDED
#define NEXTWEB_ERROR_HPP_INCLUDED

#include "nextweb/Config.hpp"

#include <exception>

namespace nextweb {

class NEXTWEB_API Error : public std::exception {

public:
	Error(char const *format, ...);
	virtual ~Error() throw ();
	virtual char const* what() const throw ();
	static int const MESSAGE_SIZE = 256;

private:
	char message_[MESSAGE_SIZE];
};

} // namespace

#endif // NEXTWEB_ERROR_HPP_INCLUDED
