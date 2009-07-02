#include "acsetup.hpp"

#include <cstdio>

#include "nextweb/Error.hpp"

namespace nextweb {

Error::Error(char const *msg)
{
	snprintf(message_, MESSAGE_SIZE, "%s", msg);
}

Error::~Error() throw () {
}

char const*
Error::what() const throw () {
	return message_;
}

} // namespace
