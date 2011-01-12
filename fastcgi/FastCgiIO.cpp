#include "acsetup.hpp"

#include "nextweb/Error.hpp"
#include "nextweb/fastcgi/impl/FastCgiIO.hpp"

namespace nextweb { namespace fastcgi {

FastCgiIO::FastCgiIO(int socket)
{
	if (0 != FCGX_InitRequest(&request_, socket, 0)) {
		throw Error("failed to init fastcgi request");
	}
}

FastCgiIO::~FastCgiIO() {
	FCGX_Free(&request_, 1);
}

void
FastCgiIO::acceptRequest() {
	if (FCGX_Accept_r(&request_) < 0) {
		throw Error("can not accept fastcgi request");
	}
}

void
FastCgiIO::finishRequest() {
	FCGX_Finish_r(&request_);
}

char const* const*
FastCgiIO::environ() const {
	return request_.envp;
}

std::size_t
FastCgiIO::read(char *buf, std::size_t size) {
	int result = FCGX_GetStr(buf, size, request_.in);
	if (result >= 0) {
		return static_cast<std::size_t>(result);
	}
	throw Error("fastcgi request already read");
}

std::size_t
FastCgiIO::write(char const *buf, std::size_t size) {
	int result = FCGX_PutStr(buf, size, request_.out);
	if (result >= 0) {
		return static_cast<std::size_t>(result);
	}
	throw Error("fastcgi request already written");
}

}} // namespaces
