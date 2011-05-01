#include "acsetup.hpp"
#include "nextweb/fastcgi/impl/SocketPolicy.hpp"

#include <fcgiapp.h>

#include "nextweb/fastcgi/Settings.hpp"
#include "nextweb/utils/EnumerationImpl.hpp"

namespace nextweb { namespace fastcgi {

DefaultSocketPolicy::DefaultSocketPolicy()
{
}

DefaultSocketPolicy::~DefaultSocketPolicy() {
}

void
DefaultSocketPolicy::init(Settings const &set) {
	(void) set;
}

Enumeration<int>::Pointer
DefaultSocketPolicy::socketSet() const {
	return utils::makeSingleValueEnumeration<int>(0);
}

TuneableSocketPolicy::TuneableSocketPolicy()
{
}

TuneableSocketPolicy::~TuneableSocketPolicy() {
}

void
TuneableSocketPolicy::init(Settings const &set) {
}

Enumeration<int>::Pointer
TuneableSocketPolicy::socketSet() const {
	return utils::makeIterEnumeration(socketSet_.begin(), socketSet_.end());
}

}} // namespaces
