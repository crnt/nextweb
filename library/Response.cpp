#include "acsetup.hpp"
#include "nextweb/Response.hpp"
#include "details/RequestImpl.hpp"
#include "details/MultiThreaded.hpp"

namespace nextweb {

Response::Response(SharedPtr<Response::ImplType> const &impl) :
	impl_(impl)
{
}

Response::~Response() {
}

void
Response::setStatus(int status) {
}

void
Response::setCookie(Cookie<StandardSetup> const &cookie) {
}
	
std::string const&
Response::getHeader(std::string const &name) const {
}

void
Response::setHeader(std::string const &name, std::string const &value) {
}
	
std::size_t
Response::write(char const *buffer) {
}

std::size_t
Response::write(char const *buffer, std::size_t len) {
}

} // namespace
