#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/ServerImpl.hpp"


#include "nextweb/fastcgi/impl/FastCgiIO.hpp"
#include "nextweb/fastcgi/impl/RequestImpl.hpp"
#include "nextweb/fastcgi/impl/ResponseImpl.hpp"
#include "nextweb/fastcgi/impl/SocketPolicy.hpp"

namespace nextweb { namespace fastcgi {

struct ServerSetup {
	typedef FastCgiIO IOType;
	typedef RequestImpl<IOType> RequestType;
	typedef ResponseImpl<IOType> ResponseType;
	typedef TuneableSocketPolicy SocketPolicyType;
};

ServerImpl::ServerImpl()
{
}

ServerImpl::~ServerImpl() {
}

SharedPtr<ServerImpl>
ServerImpl::create(Settings const &set) {
	// return SharedPtr<ServerImpl>(new TuneableServerImpl<ServerSetup>());
}

}} // namespaces
