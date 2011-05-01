#include "acsetup.hpp"
#include "nextweb/fastcgi/HttpError.hpp"
#include "nextweb/fastcgi/impl/HttpUtils.hpp"

namespace nextweb { namespace fastcgi {

HttpError::HttpError(HttpStatus const &status) :
	Error("%s", status.message()), status_(status)
{
}

HttpError::~HttpError() throw () {
}

HttpStatus
HttpError::status() const {
	return status_;
}

}} // namespaces
