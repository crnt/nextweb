#include "acsetup.hpp"

#include "nextweb/Server.hpp"
#include "details/ServerImpl.hpp"
#include "details/ThreadPool.hpp"
#include "details/ThreadPerConnect.hpp"
#include "details/MultiThreaded.hpp"

namespace nextweb {

Server::Server()
{
}

Server::~Server() {
}

void
Server::setup() {
}

void
Server::finishRequests() {
	impl_->finishRequests();
}

void
Server::handleRequests(RuntimeSettings const &settings) {
	
	using namespace details;
	RuntimeSettings::ThreadingModel model = settings.getThreadingModel();
	
	if (RuntimeSettings::THREAD_POOL == model) {
		impl_.reset(new ConcreteServerImpl<ThreadPool<StandardSetup>, StandardSetup>());
	}
	else if (RuntimeSettings::THREAD_PER_CONNECT == model) {
		impl_.reset(new ConcreteServerImpl<ThreadPerConnect<StandardSetup>, StandardSetup>());
	}
	else {
		throw Error("unknown threading model");
	}
	impl_->handleRequests(settings);
}

} // namespace
