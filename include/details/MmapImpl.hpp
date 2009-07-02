/** @file MmapImpl.hpp */
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

#ifndef NEXTWEB_DETAILS_MMAP_IMPL_HPP_INCLUDED
#define NEXTWEB_DETAILS_MMAP_IMPL_HPP_INCLUDED

#include <cstdio>
#include <limits>
#include <iostream>
#include <algorithm>

#include "nextweb/Error.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/ReferenceCounted.hpp"

#include "details/Resource.hpp"
#include "details/SystemError.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace nextweb { namespace details {

struct DescriptorTraits {
	void destroy(int value);
	static int defaultValue();
};

template <typename Char>
class MmapImpl : public ReferenceCounted {

public:
	enum Mode { READ, RDWR };
	typedef MmapImpl<Char> Type;
	typedef SharedPtr<Type> PtrType;
	
	MmapImpl(int fdes, std::size_t window);
	MmapImpl(char const *name, Mode mode, std::size_t window);
	virtual ~MmapImpl();

	off_t size() const;
	Char& at(off_t offset);
	void resize(off_t size);
	template <typename Func> void pagedWrite(Func func, off_t size);
	
	static std::size_t const BAD_SEGMENT;
	static std::size_t const DEFAULT_WINDOW_SIZE;
	
private:
	MmapImpl(MmapImpl &);
	MmapImpl& operator = (MmapImpl &);

	void init();
	void unmapSegment();
	void remapSegment(std::size_t segment);

	int mmapFlags() const;
	int openFlags() const;

private:
	Mode mode_;
	off_t size_;
	std::size_t winsize_;
	mutable Char *pointer_;
	mutable std::size_t segment_;
	Resource<int, DescriptorTraits> fdes_;
};

template <typename Char> std::size_t const
MmapImpl<Char>::BAD_SEGMENT = std::numeric_limits<std::size_t>::max();

template <typename Char> std::size_t const
MmapImpl<Char>::DEFAULT_WINDOW_SIZE = 1024 * 1024;

template <typename Char> inline
MmapImpl<Char>::MmapImpl(int fdes, std::size_t winsize) :
	mode_(RDWR), size_(0), winsize_(winsize), pointer_(0), segment_(BAD_SEGMENT), fdes_(fdes)
{
	if (0 == winsize_) {
		winsize_ = DEFAULT_WINDOW_SIZE;
	}
	SystemError::throwUnless(fdes_);
	init();
}
	
template <typename Char> inline
MmapImpl<Char>::MmapImpl(char const *name, typename MmapImpl<Char>::Mode mode, std::size_t winsize) :
	mode_(mode), size_(0), winsize_(winsize), pointer_(0), segment_(BAD_SEGMENT)
{
	if (0 == winsize_) {
		winsize_ = DEFAULT_WINDOW_SIZE;
	}
	fdes_.reset(open(name, openFlags(), 0644));
	SystemError::throwUnless(fdes_);
	init();
}

template <typename Char> inline
MmapImpl<Char>::~MmapImpl() {
	try {
		unmapSegment();
	}
	catch (Error const &e) {
	}
}

template <typename Char> inline off_t
MmapImpl<Char>::size() const {
	return size_;
}

template <typename Char> inline Char&
MmapImpl<Char>::at(off_t index) {
	off_t base = index * sizeof(Char);
	off_t offset = (base % winsize_) / sizeof(Char), seg = base / winsize_;
	remapSegment(seg);
	return *(pointer_ + offset);
}

template <typename Char> inline void
MmapImpl<Char>::resize(off_t newsize) {
	unmapSegment();
	SystemError::throwUnless(-1 != ftruncate(fdes_.get(), newsize * sizeof(Char)));
	size_ = newsize;
	remapSegment(0);
}

template <typename Char> template <typename Func> inline void
MmapImpl<Char>::pagedWrite(Func func, off_t size) {
	
	resize(size);
	std::size_t segment = 0;
	off_t nchars = winsize_ / sizeof(Char);
	for ( ; size > 0; ++segment) {
		remapSegment(segment);
		off_t topage = std::min(size, nchars);
		for (off_t i = 0; i < topage; ) {
			i += func(static_cast<Char*>(pointer_), topage - i);
		}
		size -= topage;
	}
}

template <typename Char> inline void
MmapImpl<Char>::init() {
	struct stat fs;
	SystemError::throwUnless(-1 != fstat(fdes_.get(), &fs));
	if (0 != (fs.st_size % sizeof(Char))) {
		throw Error("bad file size");
	}
	else if (0 == fs.st_size && RDWR == mode_) {
		resize(winsize_ / sizeof(Char));
	}
	size_ = fs.st_size / sizeof(Char);
	remapSegment(0);
}

template <typename Char> inline void
MmapImpl<Char>::unmapSegment() {
	if (pointer_) {
		SystemError::throwUnless(-1 != munmap(pointer_, winsize_));
		segment_ = BAD_SEGMENT;
		pointer_ = 0;
	}
}

template <typename Char> inline void
MmapImpl<Char>::remapSegment(std::size_t segment) {
	if (segment != segment_) {
		unmapSegment();
		pointer_ = static_cast<Char*>(mmap(0, winsize_, mmapFlags(),
			MAP_SHARED, fdes_.get(), segment * winsize_));
		SystemError::throwUnless(pointer_);
		segment_ = segment;
	}
}

template <typename Char> inline int
MmapImpl<Char>::mmapFlags() const {
	return PROT_READ | PROT_WRITE;
}

template <typename Char> inline int
MmapImpl<Char>::openFlags() const {
	return O_RDWR | O_CREAT;
}

}} // namespaces

#endif // NEXTWEB_DETAILS_MMAP_IMPL_HPP_INCLUDED
