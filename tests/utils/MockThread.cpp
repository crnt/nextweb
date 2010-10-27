#include "acsetup.hpp"
#include "MockThread.hpp"

namespace nextweb { namespace tests {

MockThread::MockThread(utils::ThreadQueue<std::size_t>::Pointer const &queue, std::size_t count) :
	count_(count), queue_(queue)
{
}

MockThread::~MockThread() {
}

void
MockThread::run() {
	for (std::size_t i = 0; i < count_; ++i) {
		queue_->push(i);
	}
}

}} // namespaces
