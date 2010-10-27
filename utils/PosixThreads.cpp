#include "acsetup.hpp"
#include "nextweb/utils/PosixThreads.hpp"

#include <cerrno>

#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace utils {

static inline void
checkResult(int result) {
	if (0 != result) SystemError::throwError(result);
}

Thread::Thread()
{
}

Thread::~Thread() {
}

void
Thread::join() {
	void *retval = static_cast<void*>(0);
	checkResult(pthread_join(impl_, &retval));
}

void
Thread::start() {
	pthread_attr_t attr;
	checkResult(pthread_attr_init(&attr));
	try {
		checkResult(pthread_create(&impl_, &attr, &Thread::threadFunction, this));
		pthread_attr_destroy(&attr);
	}
	catch (std::exception const &e) {
		pthread_attr_destroy(&attr);
		throw;
	}
}

void*
Thread::threadFunction(void *thread) {
	Thread *instance = static_cast<Thread*>(thread);
	instance->run();
	return static_cast<void*>(0);
}

Condition::Condition()
{
	pthread_condattr_t attr;
	checkResult(pthread_condattr_init(&attr));
	try {
		checkResult(pthread_cond_init(&impl_, &attr));
		pthread_condattr_destroy(&attr);
	}
	catch (std::exception &e) {
		pthread_condattr_destroy(&attr);
		throw;
	}
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

MutexBase::MutexBase(int mode)
{
	pthread_mutexattr_t attr;
	checkResult(pthread_mutexattr_init(&attr));
	try {
		checkResult(pthread_mutexattr_settype(&attr, mode));
		checkResult(pthread_mutex_init(&impl_, &attr));
		pthread_mutexattr_destroy(&attr);
	}
	catch (std::exception const &e) {
		pthread_mutexattr_destroy(&attr);
		throw;
	}
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

}} // namespaces
