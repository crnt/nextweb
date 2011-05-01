#include "acsetup.hpp"
#include "nextweb/utils/StaticAssert.hpp"

namespace nextweb { namespace tests {

struct StaticAssertTest {
	NEXTWEB_STATIC_ASSERT(1);
	NEXTWEB_STATIC_ASSERT(true);
};

}} // namespaces
