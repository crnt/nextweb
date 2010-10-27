#include "acsetup.hpp"
#include "nextweb/utils/Threads.hpp"

namespace nextweb { namespace utils {

void
ThreadGroup::joinAll() {
	for (std::list<ThreadPtr>::iterator i = threads_.begin(), end = threads_.end(); i != end; ) {
		(*i)->join();
		i = threads_.erase(i);
	}
}

}} // namespaces

#ifdef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/Threads.hpp"
#endif
