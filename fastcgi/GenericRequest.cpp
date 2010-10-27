#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/GenericRequest.hpp"

#include <cstdlib>

namespace nextweb { namespace fastcgi {

void
MemoryTraits::destroy(char *ptr) {
	free(ptr);
}

char*
MemoryTraits::defaultValue() {
	return static_cast<char*>(0);
}

}} // namespaces
