#include "acsetup.hpp"

#include "nextweb/Error.hpp"
#include "nextweb/fastcgi/impl/FCGI.hpp"

namespace nextweb { namespace fastcgi {

FCGI::FCGI(int sock) :
	flags_(0)
{
	if (0 != FCGX_InitRequest(&request_, sock, 0)) {
		throw Error("failed to init fastcgi request");
	}
	setFlag(REQUEST_INITIALIZED);
}

FCGI::~FCGI() {
	if (isFlagSet(REQUEST_INITIALIZED)) {
		FCGX_Free(&request_, 1);
	}
}

void
FCGI::accept() {
	checkFlag(REQUEST_INITIALIZED);
	if (FCGX_Accept_r(&request_) < 0) {
		
	}
	setFlag(REQUEST_ACCEPTED);
}

void
FCGI::finish() {
	checkFlag(REQUEST_ACCEPTED);
	FCGX_Finish_r(&request_);
}

std::size_t
FCGI::read(char *buf, std::size_t size) {
	checkFlag(REQUEST_ACCEPTED);
	int result = FCGX_GetStr(buf, size, request_.in);
	if (result >= 0) {
		return static_cast<std::size_t>(result);
	}
	throw Error("fastcgi request already read");
}

std::size_t
FCGI::write(char const *buf, std::size_t size) {
	checkFlag(REQUEST_ACCEPTED);
	int result = FCGX_PutStr(buf, size, request_.out);
	if (result >= 0) {
		return static_cast<std::size_t>(result);
	}
	throw Error("fastcgi request already written");
}

inline void
FCGI::setFlag(int flag) {
	flags_ |= flag;
}

inline void
FCGI::checkFlag(int flag) const {
	if (!isFlagSet(flag)) {
		throw Error("bad fastcgi request state");
	}
}

inline bool
FCGI::isFlagSet(int flag) const {
	return (flags_ & flag);
}

}} // namespaces
