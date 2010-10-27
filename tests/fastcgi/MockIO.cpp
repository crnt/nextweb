#include "acsetup.hpp"
#include "MockIO.hpp"

#include <cassert>
#include <exception>

namespace nextweb { namespace tests {

MockIO::MockIO() :
	post_(), env_(), strings_()
{
	env_.push_back(static_cast<char const*>(0));
	checkIsValid();
}

MockIO::~MockIO() {
}

char const* const* 
MockIO::environ() const {
	checkIsValid();
	return &env_[0];
}

void
MockIO::add(std::string const &env) {
	strings_.push_back(env);
	std::string const &copy = strings_.back();
	try {
		env_.insert(env_.begin(), copy.c_str());
	}
	catch (std::exception const &e) {
		strings_.pop_back();
		throw;
	}
	checkIsValid();
}

void
MockIO::checkIsValid() const {
	assert(env_.size() == strings_.size() + 1);
	for (std::size_t i = 0; i < env_.size() - 1; ++i) {
		assert(static_cast<char const*>(0) != env_[i]);
	}
	assert(static_cast<char const*>(0) == env_.back());
}

}} // namespaces
