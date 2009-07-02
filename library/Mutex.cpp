#include "acsetup.hpp"

#include <cerrno>
#include <cstring>

#include "details/Mutex.hpp"
#include "details/Resource.hpp"
#include "details/SystemError.hpp"

namespace nextweb { namespace details {

static inline void
checkResult(int result) {
	if (0 != result) SystemError::throwError(result);
}

struct MutexAttrTraits {
	void destroy(pthread_mutexattr_t *attr);
	static pthread_mutexattr_t* defaultValue();
};

MutexBase::MutexBase(int mode)
{
	pthread_mutexattr_t mem;
	memset(&mem, 0, sizeof(pthread_mutexattr_t));
	checkResult(pthread_mutexattr_init(&mem));
	Resource<pthread_mutexattr_t*, MutexAttrTraits> attr(&mem);
	checkResult(pthread_mutexattr_settype(attr.get(), mode));
	checkResult(pthread_mutex_init(&impl_, attr.get()));
}

MutexBase::~MutexBase() {
	pthread_mutex_destroy(&impl_);
}

void
MutexBase::lock() {
	checkResult(pthread_mutex_lock(&impl_));
}

void
MutexBase::unlock() {
	checkResult(pthread_mutex_unlock(&impl_));
}

bool
MutexBase::trylock() {
	int result = pthread_mutex_trylock(&impl_);
	if (EBUSY == result) {
		return false;
	}
	checkResult(result);
	return true;
}

inline pthread_mutexattr_t*
MutexAttrTraits::defaultValue() {
	return static_cast<pthread_mutexattr_t*>(0);
}

inline void
MutexAttrTraits::destroy(pthread_mutexattr_t *attr) {
	checkResult(pthread_mutexattr_destroy(attr));
}

}} // namespaces
