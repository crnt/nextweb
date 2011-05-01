#include "acsetup.hpp"
#include "nextweb/fastcgi/Server.hpp"
#include "nextweb/fastcgi/impl/ServerImpl.hpp"

namespace nextweb { namespace fastcgi {

Server::Server()
{
}

Server::~Server() {
}

void
Server::stop() {
	impl_->stop();
}

void
Server::start(Settings const &set, SharedPtr<Logger> const &log) {
	impl_->start(set, log);
}

void
Server::addHandler(std::string const &url, SharedPtr<RequestHandler> const &handler) {
	impl_->addHandler(url, handler);
}

}} // namespaces
