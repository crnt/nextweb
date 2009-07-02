#include "acsetup.hpp"

#include "details/Thread.hpp"
#include "details/Resource.hpp"
#include "details/SystemError.hpp"

namespace nextweb { namespace details {

static inline void
checkResult(int result) {
	if (0 != result) SystemError::throwError(result);
}

struct ThreadAttrTraits {
	void destroy(pthread_attr_t *attr);
	static pthread_attr_t* defaultValue();
};

Thread::Thread()
{
}

Thread::~Thread() {
}

void
Thread::join() {
	void *retval;
	checkResult(pthread_join(impl_, &retval));
}

void
Thread::start() {

	pthread_attr_t mem;
	memset(&mem, 0, sizeof(pthread_attr_t));
	Resource<pthread_attr_t*, ThreadAttrTraits> attr(&mem);
	
	checkResult(pthread_attr_init(attr.get()));
	checkResult(pthread_create(&impl_, attr.get(), &Thread::threadFunction, this));
}

void*
Thread::threadFunction(void *thread) {
	Thread *instance = static_cast<Thread*>(thread);
	SharedPtr<Thread> holder(instance);
	holder->run();
	return 0;
}

inline pthread_attr_t*
ThreadAttrTraits::defaultValue() {
	return static_cast<pthread_attr_t*>(0);
}

inline void
ThreadAttrTraits::destroy(pthread_attr_t *attr) {
	checkResult(pthread_attr_destroy(attr));
}

}} // namespaces
