/** @file ReadableFile.hpp */
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

#ifndef NEXTWEB_DETAILS_READABLE_FILE_HPP_INCLUDED
#define NEXTWEB_DETAILS_READABLE_FILE_HPP_INCLUDED

#include <cstdio>
#include <cstddef>
#include <iterator>

#include "details/MmapImpl.hpp"
#include "details/MmapIterator.hpp"
#include "details/MetaPrograms.hpp"

namespace nextweb { namespace details {

template <typename Char>
class ReadableFile {

public:
	ReadableFile(char const *name, std::size_t winsize = 0);
	virtual ~ReadableFile();
	
	typedef typename RemoveConst<Char>::Type CharType;
	
	typedef off_t size_type;
	typedef CharType const value_type;
	
	typedef MmapIterator<value_type> const_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef const_iterator iterator;
	typedef const_reverse_iterator reverse_iterator;

	size_type size() const;
	value_type& at(size_type index) const;
	value_type& operator [] (size_type index) const;

	const_iterator end() const;
	const_iterator begin() const;
	const_reverse_iterator rend() const;
	const_reverse_iterator rbegin() const;

private:
	void checkIndex(size_type index) const;

private:
	typename MmapImpl<CharType>::PtrType impl_;
};


template <typename Char> inline 
ReadableFile<Char>::ReadableFile(char const *name, std::size_t winsize) :
	impl_(new MmapImpl<CharType>(name, MmapImpl<CharType>::READ, winsize))
{
}

template <typename Char> inline 
ReadableFile<Char>::~ReadableFile()
{
}

template <typename Char> inline typename ReadableFile<Char>::size_type
ReadableFile<Char>::size() const {
	return impl_->size();
}

template <typename Char> inline typename ReadableFile<Char>::value_type&
ReadableFile<Char>::at(typename ReadableFile<Char>::size_type index) const {
	checkIndex(index);
	return impl_->at(index);
}

template <typename Char> inline typename ReadableFile<Char>::value_type&
ReadableFile<Char>::operator [] (typename ReadableFile<Char>::size_type index) const {
	return impl_->at(index);
}

template <typename Char> inline typename ReadableFile<Char>::const_iterator
ReadableFile<Char>::end() const {
	return const_iterator(impl_, impl_->size());
}

template <typename Char> inline typename ReadableFile<Char>::const_iterator
ReadableFile<Char>::begin() const {
	return const_iterator(impl_, 0);
}

template <typename Char> inline typename ReadableFile<Char>::const_reverse_iterator
ReadableFile<Char>::rend() const {
	return reverse_iterator(begin());
}

template <typename Char> inline typename ReadableFile<Char>::const_reverse_iterator
ReadableFile<Char>::rbegin() const {
	return reverse_iterator(end());
}

template <typename Char> inline void
ReadableFile<Char>::checkIndex(typename ReadableFile<Char>::size_type index) const {
	if (index >= size()) {
		throw Error("index is too large");
	}
}

}} // namespaces

#endif // NEXTWEB_DETAILS_READABLE_FILE_HPP_INCLUDED
