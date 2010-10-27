#include "acsetup.hpp"
#include "nextweb/fastcgi/Cookie.hpp"

#include <sstream>

#include "nextweb/Error.hpp"
#include "nextweb/fastcgi/impl/UrlEncode.hpp"

namespace nextweb { namespace fastcgi {

Cookie::Cookie() :
	expires_(HttpDate::BAD), name_(), value_()
{
}

Cookie::~Cookie() {
}

Cookie::Cookie(std::string const &name, std::string const &value) :
	expires_(HttpDate::BAD), name_(name), value_(value)
{
	if (name_.empty()) {
		throw Error("trying to set empty cookie");
	}
}

std::string const&
Cookie::name() const {
	return name_;
}
std::string const&
Cookie::value() const {
	return value_;
}

bool
Cookie::secure() const {
	return secure_;
}

void
Cookie::setSecure(bool value) {
	secure_ = value;
}

std::string const&
Cookie::path() const {
	return path_;
}

void
Cookie::setPath(std::string const &value) {
	path_ = value;
}

std::string const&
Cookie::domain() const {
	return domain_;
}

void
Cookie::setDomain(std::string const &value) {
	domain_ = value;
}
HttpDate const&
Cookie::expires() const {
	return expires_;
}

void
Cookie::setExpires(HttpDate const& value) {
	expires_ = value;
}

void
Cookie::setExpirePeriod(std::string const &value) {
	expires_ = HttpDate::fromPeriod(value);
}

std::string
Cookie::str() const {
	
	if (name_.empty()) {
		throw Error("trying to set empty cookie");
	}
	
	std::stringstream stream;
	stream << urlencode(name_) << '=' << urlencode(value_);
	
	if (!domain_.empty()) {
		stream << "; domain=" << domain_;
	}
	if (!path_.empty()) {
		stream << "; path=" << path_;
	}
	if (HttpDate::BAD != expires_) {
		stream <<  "; expires=" << expires_;
	}
	if (secure_) {
		stream << "; secure";
	}
	return stream.str();
}

}} // namespaces
