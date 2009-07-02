#include "acsetup.hpp"

#include <cassert>

#include "details/Environment.hpp"

namespace nextweb { namespace details {

Environment::Environment(FCGX_Request *request) :
	request_(request)
{
	assert(request_);
}
 
Environment::~Environment() {
}

Environment::const_iterator
Environment::end() const {
	return EnvironIterator(static_cast<char const* const*>(0));
}

Environment::const_iterator
Environment::begin() const {
	return EnvironIterator(static_cast<char const* const*>(request_->envp));
}
	
std::size_t
Environment::operator () (char *buffer, std::size_t size) const {
	return FCGX_GetStr(buffer, size, request_->in);
}

}} // namespaces
