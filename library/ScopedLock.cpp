#include "acsetup.hpp"
#include "details/ScopedLock.hpp"

namespace nextweb { namespace details {

LockError::LockError(char const *message) :
	Error(message)
{
}

LockError::~LockError() throw () {
}

}} // namespaces
