/** @file MmapIterator.hpp */
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

#ifndef NEXTWEB_DETAILS_MMAP_ITERATOR_HPP_INCLUDED
#define NEXTWEB_DETAILS_MMAP_ITERATOR_HPP_INCLUDED

#include <cstdio>

#include "nextweb/Iterator.hpp"
#include "nextweb/SharedPtr.hpp"

#include "details/MmapImpl.hpp"
#include "details/MetaPrograms.hpp"

namespace nextweb { namespace details {

template <typename Char>
class MmapIterator : public Iterator<
	MmapIterator<Char>, 
	std::random_access_iterator_tag, Char, off_t>
{

public:
	typedef MmapImpl<typename RemoveConst<Char>::Type> ImplType;
	
	MmapIterator();
	MmapIterator(SharedPtr<ImplType> const &impl, off_t offset);

	void increment();
	void decrement();
	void advance(off_t offset);
	
	bool equal(MmapIterator<Char> const &other) const;
	off_t distance(MmapIterator<Char> const &other) const;
	Char& dereference();

	operator MmapIterator<Char const>() const;

private:
	off_t offset_;
	SharedPtr<ImplType> impl_;
};

template <typename Char> inline 
MmapIterator<Char>::MmapIterator() :
	offset_(0), impl_()
{
}

template <typename Char> inline 
MmapIterator<Char>::MmapIterator(SharedPtr<typename MmapIterator<Char>::ImplType> const &impl, off_t offset) :
	offset_(offset), impl_(impl)
{
}

template <typename Char> inline void
MmapIterator<Char>::increment() {
	++offset_;
}

template <typename Char> inline void
MmapIterator<Char>::decrement() {
	--offset_;
}

template <typename Char> inline void
MmapIterator<Char>::advance(off_t offset) {
	offset_ += offset;
}

template <typename Char> inline bool
MmapIterator<Char>::equal(MmapIterator<Char> const &other) const {
	return (other.impl_ == impl_) && (other.offset_ == offset_);
}

template <typename Char> inline off_t
MmapIterator<Char>::distance(MmapIterator<Char> const &other) const {
	return other.offset_ - offset_;
}

template <typename Char> inline Char&
MmapIterator<Char>::dereference() {
	return impl_->at(offset_);
}

template <typename Char> inline 
MmapIterator<Char>::operator MmapIterator<Char const>() const {
	return MmapIterator<Char const>(impl_, offset_);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_FILE_ITERATOR_HPP_INCLUDED
