// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@yandex-team.ru>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef NEXTWEB_TEMPLATES_TEMPLATE_IMPL_HPP_INCLUDED
#define NEXTWEB_TEMPLATES_TEMPLATE_IMPL_HPP_INCLUDED

#include "nextweb/Shared.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Tokenizer.hpp"

namespace nextweb { namespace templates {

class ParamsSet;

class TemplateImpl : public Shared {

public:
	TemplateImpl();
	virtual ~TemplateImpl();
	
	virtual void parse(char const *file);
	virtual void print(ParamSet const &params) const = 0;



private:
	TemplateImpl(TemplateImpl const &);
	TemplateImpl& operator = (TemplateImpl const &);
};

template <typename String>
class StringTemplateImpl : public TemplateImpl {

public:
	StringTemplateImpl();
	virtual ~StringTemplateImpl();

	virtual void parse(char const *file);
	virtual void print(ParamSet const &params) const;
	
private:
	StringTemplateImpl(StringTemplateImpl const &);
	StringTemplateImpl& operator = (StringTemplateImpl const &);

private:
};

}} // namespaces

#endif // NEXTWEB_TEMPLATES_TEMPLATE_IMPL_HPP_INCLUDED
