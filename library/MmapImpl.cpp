#include "acsetup.hpp"
#include "details/MmapImpl.hpp"

namespace nextweb { namespace details {

void
DescriptorTraits::destroy(int fdes) {
	close(fdes);
}

int
DescriptorTraits::defaultValue() {
	return -1;
}

}} // namespaces
