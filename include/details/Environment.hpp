/** @file Environment.hpp */
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

#ifndef NEXTWEB_DETAILS_ENVIRONMENT_HPP_INCLUDED
#define NEXTWEB_DETAILS_ENVIRONMENT_HPP_INCLUDED

#include <cstddef>

#include "details/EnvironIterator.hpp"

#include <fcgiapp.h>

namespace nextweb { namespace details {

class Environment {

public:
	Environment(FCGX_Request *request);
	virtual ~Environment();
	
	typedef EnvironIterator iterator;
	typedef EnvironIterator const_iterator;

	const_iterator end() const;
	const_iterator begin() const;
	
	std::size_t operator () (char *buffer, std::size_t size) const;

private:
	FCGX_Request *request_;
};

}} // namespaces

#endif // NEXTWEB_DETAILS_ENVIRONMENT_HPP_INCLUDED
