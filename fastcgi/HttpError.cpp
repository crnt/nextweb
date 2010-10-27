#include "acsetup.hpp"
#include "nextweb/fastcgi/HttpError.hpp"
#include "nextweb/fastcgi/impl/HttpUtils.hpp"

namespace nextweb { namespace fastcgi {

unsigned short const
HttpError::BAD_REQUEST = 400;

unsigned short const
HttpError::UNAUTHORIZED = 401;

unsigned short const
HttpError::PAYMENT_REQUIRED = 402;

unsigned short const
HttpError::FORBIDDEN = 403;

unsigned short const
HttpError::NOT_FOUND = 404;

unsigned short const
HttpError::METHOD_NOT_ALLOWED = 405;

unsigned short const
HttpError::ENTITY_TOO_LARGE = 413;

unsigned short const
HttpError::RANGE_NOT_SATISFIABLE = 416;

unsigned short const
HttpError::INTERNAL_ERROR = 500;

unsigned short const
HttpError::NOT_IMPLEMENTED = 501;

unsigned short const
HttpError::BAD_GATEWAY = 502;

unsigned short const
HttpError::SERVICE_UNAVAILABLE = 503;

unsigned short const
HttpError::GATEWAY_TIMEOUT = 504;

unsigned short const
HttpError::VERSION_NOT_SUPPORTED = 505;

HttpError::HttpError(unsigned short code) :
	Error("%s", statusToString(code)), code_(code)
{
}

HttpError::~HttpError() throw () {
}

unsigned short
HttpError::code() const {
	return code_;
}

}} // namespaces
