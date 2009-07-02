/** @file MockIterator.hpp */
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

#ifndef NEXTWEB_TESTS_MOCK_ITERATOR_HPP_INCLUDED
#define NEXTWEB_TESTS_MOCK_ITERATOR_HPP_INCLUDED

#include "nextweb/Iterator.hpp"

namespace nextweb { namespace tests {

template <typename Iter>
class MockIterator : public Iterator<
	MockIterator<Iter>, 
	typename std::iterator_traits<Iter>::iterator_category, 
	typename std::iterator_traits<Iter>::value_type> 
{

public:
	MockIterator();
	MockIterator(Iter iter);

	void increment();
	void decrement();
	bool equal(MockIterator const &other) const;
	typename std::iterator_traits<Iter>::reference dereference();

	void advance(std::ptrdiff_t size);
	std::ptrdiff_t distance(MockIterator<Iter> const &other) const;

private:
	Iter iter_;
};

template <typename Iter> inline
MockIterator<Iter>::MockIterator() :
	iter_()
{
}

template <typename Iter> inline 
MockIterator<Iter>::MockIterator(Iter iter) :
	iter_(iter)
{
}

template <typename Iter> inline void
MockIterator<Iter>::increment() {
	++iter_;
}

template <typename Iter> inline void
MockIterator<Iter>::decrement() {
	--iter_;
}

template <typename Iter> inline bool
MockIterator<Iter>::equal(MockIterator const &other) const {
	return iter_ == other.iter_;
}

template <typename Iter> inline typename std::iterator_traits<Iter>::reference
MockIterator<Iter>::dereference() {
	return *iter_;
}

template <typename Iter> inline void
MockIterator<Iter>::advance(std::ptrdiff_t size) {
	iter_ += size;
}

template <typename Iter> inline std::ptrdiff_t
MockIterator<Iter>::distance(MockIterator<Iter> const &other) const {
	return std::distance(iter_, other.iter_);
}

}} // namespaces

#endif // NEXTWEB_TESTS_MOCK_ITERATOR_HPP_INCLUDED
