#include "acsetup.hpp"
#include "nextweb/fastcgi/HttpStatus.hpp"

namespace nextweb { namespace fastcgi {

HttpStatus const HttpStatus::BAD(0);
HttpStatus const HttpStatus::OK(200);
HttpStatus const HttpStatus::CREATED(201);
HttpStatus const HttpStatus::ACCEPTED(202);
HttpStatus const HttpStatus::NON_AUTHORITATIVE(203);
HttpStatus const HttpStatus::NO_CONTENT(204);
HttpStatus const HttpStatus::MOVED_PERMANENTLY(301);
HttpStatus const HttpStatus::MOVED_TEMPORARILY(302);
HttpStatus const HttpStatus::SEE_OTHER(303);
HttpStatus const HttpStatus::NOT_MODIFIED(304);
HttpStatus const HttpStatus::BAD_REQUEST(400);
HttpStatus const HttpStatus::UNAUTHORIZED(401);
HttpStatus const HttpStatus::PAYMENT_REQUIRED(402);
HttpStatus const HttpStatus::FORBIDDEN(403);
HttpStatus const HttpStatus::NOT_FOUND(404);
HttpStatus const HttpStatus::METHOD_NOT_ALLOWED(405);
HttpStatus const HttpStatus::ENTITY_TOO_LARGE(413);
HttpStatus const HttpStatus::RANGE_NOT_SATISFIABLE(416);
HttpStatus const HttpStatus::INTERNAL_ERROR(500);
HttpStatus const HttpStatus::NOT_IMPLEMENTED(501);
HttpStatus const HttpStatus::BAD_GATEWAY(502);
HttpStatus const HttpStatus::SERVICE_UNAVAILABLE(503);
HttpStatus const HttpStatus::GATEWAY_TIMEOUT(504);
HttpStatus const HttpStatus::VERSION_NOT_SUPPORTED(505);

HttpStatus::HttpStatus(unsigned short code) :
	code_(code)
{
}

char const*
HttpStatus::message() const {

	switch (code_) {
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";

		case 301: return "Moved Permanently";
		case 302: return "Moved Temporarily";
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

}} // namespaces

#ifdef NEXTWEB_DEBUG
#include "nextweb/inlines/fastcgi/HttpStatus.hpp"
#endif 
