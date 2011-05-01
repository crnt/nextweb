#include "acsetup.hpp"

#include <cerrno>
#include <cstring>

#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace utils {

inline char const*
wrapStrerror(char const *buffer, char const *) {
	return buffer;
}

inline char const*
wrapStrerror(int, char const *buffer) {
	return buffer;
}

SystemError::SystemError(char const *msg) :
	Error(msg)
{
}

void
SystemError::throwError(int error) {
	char buffer[Error::MESSAGE_SIZE];
	char const *res = wrapStrerror(strerror_r(error, buffer, sizeof(buffer)), buffer);
	throw SystemError(res);
}

void
SystemError::throwUnless(bool condition) {
	if (!condition) {
		throwError(errno);
	}
}

}} // namespaces
