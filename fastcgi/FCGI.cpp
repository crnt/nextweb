#include "acsetup.hpp"

#include "nextweb/Error.hpp"
#include "nextweb/fastcgi/impl/FCGI.hpp"

namespace nextweb { namespace fastcgi {

FCGI::FCGI(int socket)
{
	if (0 != FCGX_InitRequest(&request_, socket, 0)) {
		throw Error("failed to init fastcgi request");
	}
}

FCGI::~FCGI() {
	FCGX_Free(&request_, 1);
}

void
FCGI::accept() {
	FCGX_Accept_r(&request_) < 0;
}

void
FCGI::finish() {
	FCGX_Finish_r(&request_);
}

std::size_t
FCGI::read(char *buf, std::size_t size) {
	int result = FCGX_GetStr(buf, size, request_.in);
	if (result >= 0) {
		return static_cast<std::size_t>(result);
	}
	throw Error("fastcgi request already read");
}

std::size_t
FCGI::write(char const *buf, std::size_t size) {
	int result = FCGX_PutStr(buf, size, request_.out);
	if (result >= 0) {
		return static_cast<std::size_t>(result);
	}
	throw Error("fastcgi request already written");
}

}} // namespaces
