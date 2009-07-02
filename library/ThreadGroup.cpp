#include "acsetup.hpp"
#include "details/ThreadGroup.hpp"

namespace nextweb { namespace details {

ThreadGroup::ThreadGroup()
{
}

ThreadGroup::~ThreadGroup() {
	join();
}

void
ThreadGroup::join() {
	for (std::list<ThreadPtr>::iterator i = threads_.begin(), end = threads_.end(); i != end; ++i) {
		(*i)->join();
	}
}

}} // namespace
