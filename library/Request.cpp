#include "acsetup.hpp"
#include "nextweb/Request.hpp"
#include "details/RequestImpl.hpp"
#include "details/MultiThreaded.hpp"

namespace nextweb {

Request::Request(SharedPtr<Request::ImplType> const &impl) :
	impl_(impl)
{
}

Request::~Request() {
}

bool
Request::hasArg(Request::StringType const &name) const {
	return impl_->hasArg(name);
}

Request::StringType const&
Request::getArg(Request::StringType const &name) const {
	return impl_->getArg(name);
}

Enumeration<Request::ParamType const&>::PtrType
Request::args() const {
	return impl_->args();
}

Enumeration<Request::StringType const&>::PtrType
Request::argNames() const {
	return impl_->argNames();
}

Enumeration<Request::StringType const&>::PtrType
Request::argList(Request::StringType const &name) const {
	return impl_->argList(name);
}

bool
Request::hasVar(Request::StringType const &name) const {
	return impl_->hasVar(name);
}

Request::StringType const&
Request::getVar(Request::StringType const &name) const {
	return impl_->getVar(name);
}

Enumeration<Request::ParamType const&>::PtrType
Request::vars() const {
	return impl_->vars();
}

Enumeration<Request::StringType const&>::PtrType
Request::varNames() const {
	return impl_->varNames();
}

bool
Request::hasCookie(Request::StringType const &name) const {
	return impl_->hasCookie(name);
}

Request::StringType const&
Request::getCookie(Request::StringType const &name) const {
	return impl_->getCookie(name);
}

Enumeration<Request::ParamType const&>::PtrType
Request::cookies() const {
	return impl_->cookies();
}

Enumeration<Request::StringType const&>::PtrType
Request::cookieNames() const {
	return impl_->cookieNames();
}

bool
Request::hasHttp(Request::StringType const &name) const {
	return impl_->hasHttp(name);
}

Request::StringType const&
Request::getHttp(Request::StringType const &name) const {
	return impl_->getHttp(name);
}
	
} // namespace
