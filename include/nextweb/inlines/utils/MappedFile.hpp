// Copyright (C) 2011 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef NEXTWEB_INLINES_UTILS_MAPPED_FILE_HPP_INCLUDED
#define NEXTWEB_INLINES_UTILS_MAPPED_FILE_HPP_INCLUDED

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"
#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace utils {

NEXTWEB_INLINE off_t
fileSize(int fdes) {
	struct stat fs;
	SystemError::throwUnless(-1 != fstat(fdes, &fs));
	return fs.st_size;
}

NEXTWEB_INLINE void
truncateFile(int fdes, off_t newsize) {
	SystemError::throwUnless(-1 != ftruncate(fdes, newsize));
}

NEXTWEB_INLINE int
DescriptorTraits::defaultValue() {
	return -1;
}

NEXTWEB_INLINE void
DescriptorTraits::destroy(int fdes) {
	close(fdes);
}

NEXTWEB_INLINE 
MappedFileImpl::MappedFileImpl(char const *name, bool readOnly, MappedFileImpl::SizeType pageSize) :
	size_(0), pointer_(INVALID_POINTER), readOnly_(readOnly)
{
	(void) pageSize;
	fdes_.reset(open(name, readOnly_ ? O_RDONLY : O_RDWR | O_CREAT, 0644));
	SystemError::throwUnless(fdes_);
	size_ = fileSize(fdes_.get());
	if (0 == size_ && !readOnly) {
		resize(16 * sysconf(_SC_PAGE_SIZE));
	}
	else {
		mapFile(size_);
	}
}

NEXTWEB_INLINE 
MappedFileImpl::~MappedFileImpl() {
	if (INVALID_POINTER != pointer_) {
		munmap(pointer_, size_);
	}
}

NEXTWEB_INLINE MappedFileImpl::SizeType
MappedFileImpl::size() const {
	return size_;
}

NEXTWEB_INLINE MappedFileImpl::SizeType
MappedFileImpl::pageSize() const {
	return size_;
}

NEXTWEB_INLINE void
MappedFileImpl::resize(MappedFileImpl::SizeType newsize) {
	if (readOnly_) {
		throw Error("read only mapped file");
	}
	if (INVALID_POINTER != pointer_) {
		SystemError::throwUnless(-1 != munmap(pointer_, size_));
	}
	truncateFile(fdes_.get(), newsize);
	mapFile(newsize);
	size_ = newsize;
}

NEXTWEB_INLINE void
MappedFileImpl::reservePage(MappedFileImpl::SizeType position) {
	(void) position;
}

NEXTWEB_INLINE void
MappedFileImpl::releasePage(MappedFileImpl::SizeType position) throw () {
	(void) position;
}

NEXTWEB_INLINE void
MappedFileImpl::mapFile(MappedFileImpl::SizeType size) {
	pointer_ = mmap(0, size, readOnly_ ? PROT_READ : PROT_READ | PROT_WRITE, MAP_SHARED, fdes_.get(), 0);
	SystemError::throwUnless(INVALID_POINTER != pointer_);
}

NEXTWEB_INLINE MappedFileImpl::Pointer
MappedFileImpl::at(MappedFileImpl::SizeType position) {
	assert(position < size());
	return static_cast<char*>(pointer_) + position;
}

NEXTWEB_INLINE MappedFileImpl::Pointer
MappedFileImpl::at(MappedFileImpl::SizeType position) const {
	assert(position < size());
	return static_cast<char*>(pointer_) + position;
}

}} // namespaces

#endif // NEXTWEB_INLINES_UTILS_MAPPED_FILE_HPP_INCLUDED
