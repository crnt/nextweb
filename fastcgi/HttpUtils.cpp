#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/HttpUtils.hpp"

#include "nextweb/utils/CharTraits.hpp"

namespace nextweb { namespace fastcgi {

std::string const HttpConstants::GET("GET");
std::string const HttpConstants::HEAD("HEAD");
std::string const HttpConstants::POST("POST");
std::string const HttpConstants::METHOD("REQUEST_METHOD");
std::string const HttpConstants::STATUS("Status");
std::string const HttpConstants::EXPIRES("Expires");
std::string const HttpConstants::LOCATION("Location");
std::string const HttpConstants::SET_COOKIE("Set-Cookie");
std::string const HttpConstants::EMPTY_STRING;
std::string const HttpConstants::PATH_INFO_VAR_NAME("PATH_INFO");
std::string const HttpConstants::CONTENT_TYPE_VAR_NAME("CONTENT_TYPE");
std::string const HttpConstants::CONTENT_LENGTH_VAR_NAME("HTTP_CONTENT_LENGTH");

utils::Range<char const*> const HttpConstants::NN = utils::makeRange("\n\n");
utils::Range<char const*> const HttpConstants::RNRN = utils::makeRange("\r\n\r\n");
utils::Range<char const*> const HttpConstants::BOUNDARY = utils::makeRange("boundary");
utils::Range<char const*> const HttpConstants::FORM_DATA = utils::makeRange("form-data");
utils::Range<char const*> const HttpConstants::MULTIPART_ENDING = utils::makeRange("--");
utils::Range<char const*> const HttpConstants::MULTIPART_FORM_DATA = utils::makeRange("multipart/form-data");
utils::Range<char const*> const HttpConstants::CONTENT_TYPE = utils::makeRange("Content-Type");
utils::Range<char const*> const HttpConstants::CONTENT_DISP = utils::makeRange("Content-Disposition");
utils::Range<char const*> const HttpConstants::COOKIE_VAR_NAME = utils::makeRange("HTTP_COOKIE");
utils::Range<char const*> const HttpConstants::QUERY_STRING_VAR_NAME = utils::makeRange("QUERY_STRING");
utils::Range<char const*> const HttpConstants::NAME = utils::makeRange("name");
utils::Range<char const*> const HttpConstants::FILE_NAME = utils::makeRange("filename");

std::string
makeVarName(std::string const &header) {
	std::string result("HTTP_");
	result.reserve(result.size() + header.size());
	for (std::string::const_iterator i = header.begin(), end = header.end(); i != end; ++i) {
		if (utils::CharTraits<char>::isAlpha(*i)) {
			result.push_back(utils::CharTraits<char>::toUpper(*i));
		}
		else if ('-' == (*i)) {
			result.push_back('_');
		}
	}
	return result;
}

}} // namespaces
