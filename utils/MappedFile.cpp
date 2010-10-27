#include "acsetup.hpp"
#include "nextweb/utils/MappedFile.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "nextweb/Error.hpp"
#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace utils {

MappedFileImpl::Pointer const
MappedFileImpl::INVALID_POINTER = reinterpret_cast<MappedFileImpl::Pointer>(-1);

off_t
fileSize(int fdes) {
	struct stat fs;
	SystemError::throwUnless(-1 != fstat(fdes, &fs));
	return fs.st_size;
}

void
truncateFile(int fdes, off_t newsize) {
	SystemError::throwUnless(-1 != ftruncate(fdes, newsize));
}

int
DescriptorTraits::defaultValue() {
	return -1;
}

void
DescriptorTraits::destroy(int fdes) {
	close(fdes);
}

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

MappedFileImpl::~MappedFileImpl() {
	if (INVALID_POINTER != pointer_) {
		munmap(pointer_, size_);
	}
}

MappedFileImpl::SizeType
MappedFileImpl::size() const {
	return size_;
}

MappedFileImpl::SizeType
MappedFileImpl::pageSize() const {
	return size_;
}

void
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

void
MappedFileImpl::reservePage(MappedFileImpl::SizeType position) {
	(void) position;
}

void
MappedFileImpl::releasePage(MappedFileImpl::SizeType position) throw () {
	(void) position;
}

void
MappedFileImpl::mapFile(MappedFileImpl::SizeType size) {
	pointer_ = mmap(0, size, readOnly_ ? PROT_READ : PROT_READ | PROT_WRITE, MAP_SHARED, fdes_.get(), 0);
	SystemError::throwUnless(INVALID_POINTER != pointer_);
}

}} // namespaces
