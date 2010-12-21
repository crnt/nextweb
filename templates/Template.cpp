#include "acsetup.hpp"
#include "nextweb/templates/Template.hpp"
#include "nextweb/templates/impl/TemplateImpl.hpp"

namespace nextweb { namespace templates {

Template::Template() :
	impl_()
{
}

Template::~Template() {
}

void
Template::parse(char const *file) {
}

void 
Template::print(ParamSet const &params) const {
	impl_->print(params);
}

}} // namespaces
