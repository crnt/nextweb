#include "acsetup.hpp"

#include <cassert>

#include "details/EnvironIterator.hpp"

namespace nextweb { namespace details {

EnvironIterator::EnvironIterator() :
	ptr_(0)
{
}

EnvironIterator::EnvironIterator(char const* const* ptr) :
	ptr_(ptr)
{
}

void
EnvironIterator::increment() {
	assert(ptr_);
	++ptr_;
}

char const*
EnvironIterator::dereference() {
	assert(ptr_);
	return *ptr_;
}

bool
EnvironIterator::equal(EnvironIterator const &other) const {
	return ptr_ == other.ptr_;
}

}} // namespaces
