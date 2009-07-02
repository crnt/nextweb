#include "acsetup.hpp"

#include <cstring>

#include "details/RequestImpl.hpp"

namespace nextweb { namespace details {

RequestImplBase::RequestImplBase() :
	initialized_(false)
{
	memset(&request_, 0, sizeof(FCGX_Request));
}

RequestImplBase::~RequestImplBase() {
	if (initialized_) {
		FCGX_Free(&request_, 1);
	}
}

bool
RequestImplBase::acceptRequest() {
	return (FCGX_Accept_r(&request_) >= 0);
}

void
RequestImplBase::finishRequest() {
	FCGX_Finish_r(&request_);
}

void
RequestImplBase::initRequest(int socket) {
	if (FCGX_InitRequest(&request_, socket, 0) == 0) {
		initialized_ = true;
	}
}

Environment
RequestImplBase::environ() {
	assert(initialized_);
	return Environment(&request_);
}

}} // namespaces
