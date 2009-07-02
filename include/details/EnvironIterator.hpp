/** @file EnvironIterator.hpp */
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

#ifndef NEXTWEB_DETAILS_ENVIRON_ITERATOR_HPP_INCLUDED
#define NEXTWEB_DETAILS_ENVIRON_ITERATOR_HPP_INCLUDED

#include <iterator>

#include "nextweb/Iterator.hpp"

namespace nextweb { namespace details {

class EnvironIterator : public Iterator<
	EnvironIterator, 
	std::forward_iterator_tag, char const*, 
	std::ptrdiff_t, char const* const*, char const*>
{
public:
	EnvironIterator();
	EnvironIterator(char const* const* ptr);

	void increment();
	char const* dereference();
	bool equal(EnvironIterator const &other) const;
	
private:
	char const* const* ptr_;
};

}} // namespaces

#endif // NEXTWEB_DETAILS_ENVIRON_ITERATOR_HPP_INCLUDED
