#include "acsetup.hpp"
#include "nextweb/fastcgi/UrlEncode.hpp"

#include "nextweb/utils/Range.hpp"
#include "nextweb/fastcgi/impl/UrlEncodeImpl.hpp"

namespace nextweb { namespace fastcgi {

std::string
urlencode(char const *value) {
	return urlencode<std::string>(utils::makeRange(value));
}

std::string
urlencode(std::string const &value) {
	return urlencode<std::string, std::string>(value);
}

std::string
urldecode(char const *value) {
	return urldecode<std::string>(utils::makeRange(value));
}

std::string
urldecode(std::string const &value) {
	return urldecode<std::string, std::string>(value);
}

}} // namespaces
