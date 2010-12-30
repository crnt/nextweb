#include "acsetup.hpp"
#include "nextweb/templates/ParamSet.hpp"

#include <sstream>
#include <algorithm>

#include "nextweb/Error.hpp"

namespace nextweb { namespace templates {

Param::Param()
{
}

Param::~Param()
{
}

Param::Param(Param const &)
{
}

Param&
Param::operator = (Param const &) {
	return *this;
}

SubParam::SubParam(Param *parent, std::string const &prefix) :
	parent_(parent), prefix_(prefix)
{
}

SubParam::~SubParam() {
}

SubParam::SubParam(SubParam const &other) :
	Param(), parent_(other.parent_), prefix_(other.prefix_)
{
}

SubParam&
SubParam::operator = (SubParam const &other) {
	SubParam copy(other);
	swap(copy);
	return *this;
}

void
SubParam::swap(SubParam &other) throw () {
	std::swap(parent_, other.parent_);
	std::swap(prefix_, other.prefix_);
}

void
SubParam::operator = (std::string const &value) {
	setValue(value);
}

SubParam
SubParam::operator [] (std::string const &prefix) {
	return SubParam(this, prefix);
}

std::string const&
SubParam::get(std::string const &name) const {
	std::stringstream stream;
	stream << prefix_ << "." << name;
	return parent_->get(stream.str());
}

void
SubParam::set(std::string const &name, std::string const &value) {
	std::stringstream stream;
	stream << prefix_ << "." << name;
	parent_->set(stream.str(), value);
}

std::string const&
SubParam::getValue() const {
	return parent_->get(prefix_);
}

void
SubParam::setValue(std::string const &value) {
	parent_->set(prefix_, value);
}

ParamSet::ParamSet()
{
}

ParamSet::~ParamSet() {
}

std::string const&
ParamSet::get(std::string const &name) const {
	std::map<std::string, std::string>::const_iterator i = values_.find(name); 
	if (values_.end() == i) {
		throw Error("param named [%s] does not exist", name.c_str());
	}
	return i->second;
}

void
ParamSet::set(std::string const &name, std::string const &value) {
	values_[name] = value;
}

}} // namespaces
