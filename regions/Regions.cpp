#include "acsetup.hpp"

#include "Regions.hpp"
#include "RegionsImpl.hpp"
#include "details/MultiThreaded.hpp"

namespace nextweb { namespace regions {

Regions::Regions(char const *name) :
	impl_(new ImplType(name))
{
}

Regions::~Regions() {
}

}} // namespaces
