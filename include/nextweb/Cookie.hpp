/** @file Cookie.hpp */
// nextweb - modern web framework for Python and C++
// Copyright (C) 2009 Oleg Obolenskiy <highpower@mail.ru>

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

#ifndef NEXTWEB_COOKIE_HPP_INCLUDED
#define NEXTWEB_COOKIE_HPP_INCLUDED

#include <ctime>

#include "nextweb/Config.hpp"
#include "nextweb/HttpUtils.hpp"

namespace nextweb {

template <typename Setup>
class NEXTWEB_API Cookie
{
public:
	typedef typename Setup::StringType StringType;
	
	Cookie(StringType &name, StringType &value);
	virtual ~Cookie();
	
	bool isSecure() const;
	void setSecure(bool value);
	
	time_t getExpires() const;
	void setExpires(time_t value);

	StringType const& getName() const;
	StringType const& getValue() const;
	
	StringType const& getPath() const;
	void setPath(StringType const &value);
	
	StringType const& getDomain() const;
	void setDomain(StringType const &value);

	StringType toString() const;
	bool operator < (const Cookie<Setup> &cookie) const;

private:
	bool secure_;
	time_t expires_;
	StringType name_, value_;
	StringType path_, domain_;
};

template <typename Setup> inline 
Cookie<Setup>::Cookie(typename Cookie<Setup>::StringType &name, typename Cookie<Setup>::StringType &value) :
	name_(name), value_(value)
{
}

template <typename Setup> inline
Cookie<Setup>::~Cookie() {
}

template <typename Setup> inline bool
Cookie<Setup>::isSecure() const {
	return secure_;
}

template <typename Setup> inline void
Cookie<Setup>::setSecure(bool value) {
	secure_ = value;
}

template <typename Setup> inline time_t
Cookie<Setup>::getExpires() const {
	return expires_;
}

template <typename Setup> inline void
Cookie<Setup>::setExpires(time_t value) {
	expires_ = value;
}

template <typename Setup> inline typename Cookie<Setup>::StringType const&
Cookie<Setup>::getName() const {
	return name_;
}

template <typename Setup> inline typename Cookie<Setup>::StringType const&
Cookie<Setup>::getValue() const {
	return value_;
}

template <typename Setup> inline typename Cookie<Setup>::StringType const&
Cookie<Setup>::getPath() const {
	return path_;
}

template <typename Setup> inline void
Cookie<Setup>::setPath(typename Cookie<Setup>::StringType const &value) {
	path_ = value;
}

template <typename Setup> inline typename Cookie<Setup>::StringType const&
Cookie<Setup>::getDomain() const {
	return domain_;
}

template <typename Setup> inline void
Cookie<Setup>::setDomain(typename Cookie<Setup>::StringType const &value) {
	domain_ = value;
}

template <typename Setup> inline typename Cookie<Setup>::StringType
Cookie<Setup>::toString() const {
	StringType result;
}

template <typename Setup> inline bool
Cookie<Setup>::operator < (const Cookie<Setup> &cookie) const {
	return name_ < cookie.name_;
}

} // namespace

#endif // NEXTWEB_COOKIE_HPP_INCLUDED
