#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/ServerImpl.hpp"

#include "nextweb/fastcgi/RequestHandler.hpp"
#include "nextweb/fastcgi/impl/RequestImpl.hpp"
#include "nextweb/fastcgi/impl/ResponseImpl.hpp"

namespace nextweb { namespace fastcgi {

ServerImpl::ServerImpl()
{
}

ServerImpl::~ServerImpl() {
}

SharedPtr<ServerImpl>
ServerImpl::create(Settings const &set) {
	return SharedPtr<ServerImpl>();
}

}} // namespaces
