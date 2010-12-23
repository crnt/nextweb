// nextweb - modern web framework for Python and C++
// Copyright (C) 2011 Oleg Obolenskiy <highpower@yandex-team.ru>

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

#ifndef NEXTWEB_TEMPLATES_PARAM_SET_HPP_INCLUDED
#define NEXTWEB_TEMPLATES_PARAM_SET_HPP_INCLUDED

#include <map>
#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/utils/StringConverters.hpp"

namespace nextweb { namespace templates {

class NEXTWEB_API Param {

public:
	Param();
	virtual ~Param();
	virtual std::string const& get(std::string const &name) const = 0;
	virtual void set(std::string const &name, std::string const &value) = 0;
	
protected:
	Param(Param const &);
	Param& operator = (Param const &);
};

class NEXTWEB_API SubParam : public Param {

public:
	SubParam(Param *parent, std::string const &prefix);
	virtual ~SubParam();
	virtual std::string const& get(std::string const &name) const;
	virtual void set(std::string const &name, std::string const &value);
	
	SubParam(SubParam const &other);
	SubParam& operator = (SubParam const &other);
	void swap(SubParam &other) throw ();

	template <typename T> T as() const;
	template <typename T> void operator = (T const &value);
	template <typename T> SubParam operator [] (T const &prefix);
	
private:
	std::string const& getValue() const;
	void setValue(std::string const &value);

private:
	Param *parent_;
	std::string prefix_;
};

class NEXTWEB_API ParamSet : public Param {

public:
	ParamSet();
	virtual ~ParamSet();
	virtual std::string const& get(std::string const &name) const;
	virtual void set(std::string const &name, std::string const &value);
	
	template <typename T> SubParam operator [] (T const &prefix);
	
private:
	ParamSet(ParamSet const &);
	ParamSet& operator = (ParamSet const &);

private:
	std::map<std::string, std::string> values_;
};

template <typename T> NEXTWEB_INLINE T
SubParam::as() const {
	return utils::fromString<T>(getValue());
}

template <typename T> NEXTWEB_INLINE void
SubParam::operator = (T const &value) {
	setValue(utils::toString(value));
}

template <typename T> NEXTWEB_INLINE SubParam
SubParam::operator [] (T const &prefix) {
	return SubParam(this, utils::toString(prefix));
}

template <typename T> NEXTWEB_INLINE SubParam
ParamSet::operator [] (T const &prefix) {
	return SubParam(this, utils::toString(prefix));
}

}} // namespaces

#endif // NEXTWEB_TEMPLATES_PARAM_SET_HPP_INCLUDED
