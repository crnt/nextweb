/** @file WriteableFile.hpp */
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

#ifndef NEXTWEB_DETAILS_WRITEABLE_FILE_HPP_INCLUDED
#define NEXTWEB_DETAILS_WRITEABLE_FILE_HPP_INCLUDED

#include <cstdio>
#include <climits>
#include <cassert>
#include <cstdlib>
#include <cstddef>
#include <iterator>

#include "details/MmapImpl.hpp"
#include "details/MmapIterator.hpp"
#include "details/MetaPrograms.hpp"

namespace nextweb { namespace details {

template <typename Char>
class WriteableFile {

public:
	WriteableFile(char const *name, std::size_t winsize = 0);
	virtual ~WriteableFile();
	
	typedef typename RemoveConst<Char>::Type CharType;
	
	typedef off_t size_type;
	typedef CharType value_type;
	
	typedef MmapIterator<value_type> iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;

	typedef MmapIterator<value_type const> const_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	size_type size() const;
	void resize(size_type newsize);
	
	value_type& at(size_type index);
	value_type const& at(size_type index) const;
	
	value_type& operator [] (size_type index);
	value_type const& operator [] (size_type index) const;

	iterator end();
	const_iterator end() const;
	
	iterator begin();
	const_iterator begin() const;
	
	reverse_iterator rend();
	const_reverse_iterator rend() const;
	
	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;

	template <typename Func> void fill(Func func, size_type size);
	
	static WriteableFile<Char> temporary(std::size_t threshold);

private:
	WriteableFile(typename MmapImpl<CharType>::PtrType const &impl);
	void checkIndex(size_type index) const;
	
private:
	typename MmapImpl<CharType>::PtrType impl_;
};

template <typename Char> inline 
WriteableFile<Char>::WriteableFile(char const *name, std::size_t winsize) :
	impl_(new MmapImpl<CharType>(name, MmapImpl<CharType>::RDWR, winsize))
{
}

template <typename Char> inline 
WriteableFile<Char>::WriteableFile(typename MmapImpl<CharType>::PtrType const &impl) :
	impl_(impl)
{
	assert(impl_);
}

template <typename Char> inline 
WriteableFile<Char>::~WriteableFile()
{
}

template <typename Char> inline typename WriteableFile<Char>::size_type
WriteableFile<Char>::size() const {
	return impl_->size();
}

template <typename Char> inline void
WriteableFile<Char>::resize(typename WriteableFile<Char>::size_type newsize) {
	impl_->resize(newsize);
}

template <typename Char> inline typename WriteableFile<Char>::value_type&
WriteableFile<Char>::at(typename WriteableFile<Char>::size_type index) {
	checkIndex(index);
	return impl_->at(index);
}

template <typename Char> inline typename WriteableFile<Char>::value_type const&
WriteableFile<Char>::at(typename WriteableFile<Char>::size_type index) const {
	checkIndex(index);
	return impl_->at(index);
}

template <typename Char> inline typename WriteableFile<Char>::value_type&
WriteableFile<Char>::operator [] (typename WriteableFile<Char>::size_type index) {
	return impl_->at(index);
}

template <typename Char> inline typename WriteableFile<Char>::value_type const&
WriteableFile<Char>::operator [] (typename WriteableFile<Char>::size_type index) const {
	return impl_->at(index);
}

template <typename Char> inline typename WriteableFile<Char>::iterator
WriteableFile<Char>::end() {
	return iterator(impl_, impl_->size());
}

template <typename Char> inline typename WriteableFile<Char>::const_iterator
WriteableFile<Char>::end() const {
	return const_iterator(impl_, impl_->size());	
}

template <typename Char> inline typename WriteableFile<Char>::iterator
WriteableFile<Char>::begin() {
	return iterator(impl_, 0);
}

template <typename Char> inline typename WriteableFile<Char>::const_iterator
WriteableFile<Char>::begin() const {
	return const_iterator(impl_, 0);
}

template <typename Char> inline typename WriteableFile<Char>::reverse_iterator
WriteableFile<Char>::rend() {
	return reverse_iterator(begin());
}

template <typename Char> inline typename WriteableFile<Char>::const_reverse_iterator
WriteableFile<Char>::rend() const {
	return const_reverse_iterator(begin());
}

template <typename Char> inline typename WriteableFile<Char>::reverse_iterator
WriteableFile<Char>::rbegin() {
	return reverse_iterator(end());
}

template <typename Char> inline typename WriteableFile<Char>::const_reverse_iterator
WriteableFile<Char>::rbegin() const {
	return const_reverse_iterator(end());
}

template <typename Char> inline WriteableFile<Char>
WriteableFile<Char>::temporary(std::size_t winsize) {
	
	char pattern[_POSIX_PATH_MAX];
	Resource<int, DescriptorTraits> fdes;
	snprintf(pattern, sizeof(pattern), "/tmp/nextweb-writeable-XXXXXX");
	fdes.reset(mkstemp(pattern));
	SystemError::throwUnless(fdes);
	
	typename MmapImpl<Char>::PtrType impl(new MmapImpl<Char>(fdes.get(), winsize));
	fdes.release();
	return WriteableFile<Char>(impl);
}

template <typename Char> template <typename Func> inline void
WriteableFile<Char>::fill(Func func, typename WriteableFile<Char>::size_type size) {
	impl_->pagedWrite(func, size);
}

template <typename Char> inline void
WriteableFile<Char>::checkIndex(typename WriteableFile<Char>::size_type index) const {
	if (index >= size()) {
		throw Error("index is too large");
	}
}

}} // namespaces

#endif // NEXTWEB_DETAILS_WRITEABLE_FILE_HPP_INCLUDED
