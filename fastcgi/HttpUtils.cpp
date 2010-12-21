#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/HttpUtils.hpp"
#include "nextweb/utils/CharTraits.hpp"

namespace nextweb { namespace fastcgi {

std::string const
HttpConstants::GET("GET");

std::string const
HttpConstants::HEAD("HEAD");

std::string const
HttpConstants::POST("POST");

std::string const
HttpConstants::METHOD("REQUEST_METHOD");

std::string const
HttpConstants::PATH_INFO("PATH_INFO");

std::string const
HttpConstants::EMPTY_STRING;

std::string const
HttpConstants::CONTENT_TYPE("CONTENT_TYPE");

std::string const
HttpConstants::CONTENT_LENGTH("HTTP_CONTENT_LENGTH");

utils::Range<char const*> const
HttpConstants::NN = utils::makeRange("\n\n");

utils::Range<char const*> const
HttpConstants::RNRN = utils::makeRange("\r\n\r\n");

utils::Range<char const*> const
HttpConstants::BOUNDARY = utils::makeRange("boundary");

utils::Range<char const*> const
HttpConstants::COOKIE_VAR_NAME = utils::makeRange("HTTP_COOKIE");

utils::Range<char const*> const
HttpConstants::MULTIPART_FORM_DATA = utils::makeRange("multipart/form-data");

utils::Range<char const*> const
HttpConstants::QUERY_STRING_VAR_NAME = utils::makeRange("QUERY_STRING");

char const*
statusToString(unsigned short code) {
	switch (code) {
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";

		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";

		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "PaymentRequired";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 416: return "Requested Range Not Satisfiable";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
	}
	return "Unknown Status";
}

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
