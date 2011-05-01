#include "acsetup.hpp"
#include "MockShared.hpp"

namespace nextweb { namespace tests {

std::set<MockShared*> 
MockShared::objects_;

MockShared::MockShared()
{
	objects_.insert(this);
}

MockShared::~MockShared() {
	objects_.erase(this);
}

bool
MockShared::isLive(MockShared *object) {
	return objects_.find(object) != objects_.end();
}

MockSharedDerived::MockSharedDerived()
{
}

MockSharedDerived::~MockSharedDerived() {
}

}} // namespaces
