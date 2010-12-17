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

#ifndef NEXTWEB_TEMPLATES_PARAMS_HPP_INCLUDED
#define NEXTWEB_TEMPLATES_PARAMS_HPP_INCLUDED

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

template <typename Derived, typename Result>
struct NEXTWEB_API SubscribeImpl {
	Result operator [] (std::string const &prefix);
	template <typename T> Result operator [] (T const &prefix);
};

class NEXTWEB_API SubParam : 
	public Param, 
	public SubscribeImpl<SubParam, SubParam>
{
public:
	SubParam(Param *parent, std::string const &prefix);
	virtual ~SubParam();
	
	SubParam(SubParam const &other);
	SubParam& operator = (SubParam const &other);
	void swap(SubParam &other) throw ();

	operator std::string const& () const;
	template <typename T> operator T () const;

	void operator = (std::string const &value);
	template <typename T> void operator = (T const &value);
	
	virtual std::string const& get(std::string const &name) const;
	virtual void set(std::string const &name, std::string const &value);
	
private:
	std::string const& getValue() const;
	void setValue(std::string const &value);
	SubParam create(std::string const &name);

private:
	Param *parent_;
	std::string prefix_;
};

class NEXTWEB_API ParamSet : 
	public Param, 
	public SubscribeImpl<ParamSet, SubParam>
{
public:
	ParamSet();
	virtual ~ParamSet();
	virtual std::string const& get(std::string const &name) const;
	virtual void set(std::string const &name, std::string const &value);

private:
	ParamSet(ParamSet const &);
	ParamSet& operator = (ParamSet const &);

private:
	std::map<std::string, std::string> values_;
};

template <typename Derived, typename Result> NEXTWEB_INLINE Result
SubscribeImpl<Derived, Result>::operator [] (std::string const &prefix) {
	return Result(static_cast<Derived*>(this), prefix);
}

template <typename Derived, typename Result> template <typename T> NEXTWEB_INLINE Result
SubscribeImpl<Derived, Result>::operator [] (T const &prefix) {
	return Result(static_cast<Derived*>(this), utils::toString(prefix));
}

template <typename T> NEXTWEB_INLINE
SubParam::operator T () const {
	return utils::fromString<T>(getValue());
}

template <typename T> NEXTWEB_INLINE void
SubParam::operator = (T const &value) {
	setValue(utils::toString(value));
}

}} // namespaces

#endif // NEXTWEB_TEMPLATES_PARAMS_HPP_INCLUDED

