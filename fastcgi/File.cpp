#include "acsetup.hpp"

#include "nextweb/fastcgi/File.hpp"

#include <cassert>

#include "nextweb/fastcgi/impl/FileImpl.hpp"

namespace nextweb { namespace fastcgi {

File::File()
{
}

File::~File() {
}

File::File(SharedPtr<FileImpl> const &impl) :
	impl_(impl)
{
	assert(impl_);
}

File::File(File const &other) :
	impl_(other.impl_)
{
}

File&
File::operator = (File const &other) {
	impl_ = other.impl_;
	return *this;
}

std::streamsize
File::size() const {
	return impl_->size();
}

std::string const&
File::name() const {
	return impl_->name();
}

std::string const&
File::contentType() const {
	return impl_->contentType();
}

std::istream&
File::stream() {
	return impl_->stream();
}

void
File::save(std::string const &name) const {
	impl_->save(name);
}

}} // namespaces
