#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/NullLogger.hpp"

namespace nextweb { namespace fastcgi {

NullLogger::NullLogger()
{
}

NullLogger::~NullLogger() {
}

void
NullLogger::init(Settings const &set) {
	(void) set;
}

void
NullLogger::info(char const *format, ...) {
	(void) format;
}

void
NullLogger::debug(char const *format, ...) {
	(void) format;
}

void
NullLogger::error(char const *format, ...) {
	(void) format;
}

}} // namespaces
