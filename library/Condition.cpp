#include "acsetup.hpp"

#include <cstring>

#include "details/Resource.hpp"
#include "details/Condition.hpp"
#include "details/SystemError.hpp"

namespace nextweb { namespace details {

static inline void
checkResult(int result) {
	if (0 != result) SystemError::throwError(result);
}

struct CondAttrTraits {
	void destroy(pthread_condattr_t *attr);
	static pthread_condattr_t* defaultValue();
};

Condition::Condition()
{
	pthread_condattr_t mem;
	memset(&mem, 0, sizeof(pthread_condattr_t));
	checkResult(pthread_condattr_init(&mem));
	Resource<pthread_condattr_t*, CondAttrTraits> attr(&mem);
	checkResult(pthread_cond_init(&impl_, attr.get()));
}

Condition::~Condition() {
	pthread_cond_destroy(&impl_);
}

void
Condition::signal() {
	checkResult(pthread_cond_signal(&impl_));
}

void
Condition::broadcast() {
	checkResult(pthread_cond_broadcast(&impl_));
}

void
Condition::wait(pthread_mutex_t &mutex) {
	checkResult(pthread_cond_wait(&impl_, &mutex));
}

bool
Condition::timedWait(pthread_mutex_t &mutex, TimeSpec const &ts) {
	
}

pthread_condattr_t*
CondAttrTraits::defaultValue() {
	return static_cast<pthread_condattr_t*>(0);
}

void
CondAttrTraits::destroy(pthread_condattr_t *attr) {
	checkResult(pthread_condattr_destroy(attr));
}

}} // namespaces
