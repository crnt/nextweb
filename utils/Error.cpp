#include "acsetup.hpp"
#include "nextweb/Error.hpp"

#include <cstdio>
#include <cstdarg>

namespace nextweb {

Error::Error(char const *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(message_, MESSAGE_SIZE, format, args);
	va_end(args);
}

Error::~Error() throw () {
}

char const*
Error::what() const throw () {
	return message_;
}

} // namespace
