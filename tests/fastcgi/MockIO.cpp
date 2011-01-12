#include "acsetup.hpp"
#include "MockIO.hpp"

#include <cassert>
#include <exception>

#include "nextweb/Error.hpp"
#include "nextweb/utils/Integer.hpp"

namespace nextweb { namespace tests {

MockIO::MockIO() :
	post_(), strings_(), env_()
{
	post_.exceptions(std::ios::badbit | std::ios::failbit);
	validate();
}

MockIO::~MockIO() {
}

void
MockIO::add(std::string const &env) {
	strings_.push_back(env);
	validate();
}

void
MockIO::attachFile(char const *name) {
	post_.open(name);
	validate();
}

char const* const* 
MockIO::environ() const {
	validate();
	return &env_[0];
}

std::size_t
MockIO::read(char *buffer, std::size_t size) {
	post_.read(buffer, size);
	std::size_t result = static_cast<std::size_t>(post_.gcount());
	if (size != result) {
		throw Error("can not read %llu bytes", static_cast<utils::UInt64>(size)); 
	}
	return result;
}

std::size_t
MockIO::write(char const *buffer, std::size_t size) {
	(void) size;
	(void) buffer;
	return 0;
}

void
MockIO::validate() const {
	env_.clear();
	for (std::vector<std::string>::const_iterator i = strings_.begin(), end = strings_.end(); i != end; ++i) {
		env_.push_back(i->c_str());
	}
	env_.push_back(static_cast<char const*>(0));
}

}} // namespaces
