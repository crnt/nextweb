#include "acsetup.hpp"
#include "nextweb/utils/VarUtils.hpp"

namespace nextweb { namespace utils {

static char const
END_TOKEN[] = "}";

static char const
BEGIN_TOKEN[] = "${";

char const*
VarConstants<char>::endToken() {
	return END_TOKEN;
}

std::size_t
VarConstants<char>::endTokenSize() {
	return sizeof(END_TOKEN) - 1;
}

char const*
VarConstants<char>::beginToken() {
	return BEGIN_TOKEN;
}

std::size_t
VarConstants<char>::beginTokenSize() {
	return sizeof(BEGIN_TOKEN) - 1;
}

}} // namespaces
