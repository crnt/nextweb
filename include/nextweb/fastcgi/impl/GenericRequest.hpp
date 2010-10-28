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

#ifndef NEXTWEB_FASTCGI_GENERIC_REQUEST_HPP_INCLUDED
#define NEXTWEB_FASTCGI_GENERIC_REQUEST_HPP_INCLUDED

#include <map>
#include <string>

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"

#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/StringUtils.hpp"

#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/StaticAssert.hpp"

#include "nextweb/fastcgi/File.hpp"
#include "nextweb/fastcgi/HttpError.hpp"

#include "nextweb/fastcgi/impl/HttpUtils.hpp"
#include "nextweb/fastcgi/impl/UrlEncode.hpp"

namespace nextweb { namespace fastcgi {

template <typename IO>
class GenericRequest {

public:
	GenericRequest(IO &io, std::size_t threshold);
	virtual ~GenericRequest();

	typedef std::map<std::string, File> FileMap;
	typedef std::map<std::string, std::string> StringMap;
	typedef std::multimap<std::string, std::string> StringMultiMap;

	bool isSecure() const;
	std::string const& pathInfo() const;

	StringMap const& vars() const;
	bool hasVar(std::string const &name) const;
	std::string const& getVar(std::string const &name) const;

	StringMap const& cookies() const;
	bool hasCookie(std::string const &name) const;
	std::string const& getCookie(std::string const &name) const;

	StringMultiMap const& args() const;
	bool hasArg(std::string const &name) const;
	std::string const& getArg(std::string const &name) const;

	bool hasHeader(std::string const &name) const;
	std::string const& getHeader(std::string const &name) const;

	FileMap const& files() const;
	bool hasFile(std::string const &name) const;
	File getFile(std::string const &name) const;
	
private:
	GenericRequest(GenericRequest const &);
	GenericRequest& operator = (GenericRequest const &);

	void parse(std::size_t threshold);
	void parsePost(std::size_t threshold);
	void parseArg(utils::Range<char const*> const &range);
	void parseVar(utils::Range<char const*> const &range);
	void parseCookie(utils::Range<char const*> const &range);
	void parseCookies(utils::Range<char const*> const &range);
	void parseQueryString(utils::Range<char const*> const &range);
	
private:	
	IO &io_;
	FileMap files_;
	StringMultiMap args_;
	StringMap vars_, cookies_;
};

template <typename Map> NEXTWEB_INLINE bool
mapHas(Map const &map, typename Map::key_type const &key) {
	return map.find(key) != map.end();
}

template <typename Map> NEXTWEB_INLINE std::string const&
mapGet(Map const &map, std::string const &key) {
	
	NEXTWEB_STATIC_ASSERT((utils::IsSame<std::string, typename Map::mapped_type>::RESULT));
	NEXTWEB_STATIC_ASSERT((utils::IsSame<std::string, typename utils::RemoveConst<typename Map::key_type>::Type>::RESULT));

	typename Map::const_iterator i = map.find(key);
	return (map.end() == i) ? HttpConstants::EMPTY_STRING : i->second;
}

template <typename IO> NEXTWEB_INLINE 
GenericRequest<IO>::GenericRequest(IO &io, std::size_t threshold) :
	io_(io)
{
	parse(threshold);
}

template <typename IO> NEXTWEB_INLINE 
GenericRequest<IO>::~GenericRequest() {
}

template <typename IO> NEXTWEB_INLINE bool
GenericRequest<IO>::isSecure() const {
	return hasVar("HTTPS");
}

template <typename IO> NEXTWEB_INLINE std::string const&
GenericRequest<IO>::pathInfo() const {
	return getVar(HttpConstants::PATH_INFO_VAR_NAME);
}

template <typename IO> NEXTWEB_INLINE typename GenericRequest<IO>::StringMap const&
GenericRequest<IO>::vars() const {
	return vars_;
}

template <typename IO> NEXTWEB_INLINE bool
GenericRequest<IO>::hasVar(std::string const &name) const {
	return mapHas(vars_, name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
GenericRequest<IO>::getVar(std::string const &name) const {
	return mapGet(vars_, name);
}

template <typename IO> NEXTWEB_INLINE typename GenericRequest<IO>::StringMap const&
GenericRequest<IO>::cookies() const {
	return cookies_;
}

template <typename IO> NEXTWEB_INLINE bool
GenericRequest<IO>::hasCookie(std::string const &name) const {
	return mapHas(cookies_, name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
GenericRequest<IO>::getCookie(std::string const &name) const {
	return mapGet(cookies_, name);
}

template <typename IO> NEXTWEB_INLINE typename GenericRequest<IO>::StringMultiMap const&
GenericRequest<IO>::args() const {
	return args_;
}

template <typename IO> NEXTWEB_INLINE bool
GenericRequest<IO>::hasArg(std::string const &name) const {
	return mapHas(args_, name);
}

template <typename IO> NEXTWEB_INLINE std::string const&
GenericRequest<IO>::getArg(std::string const &name) const {
	return mapGet(args_, name);
}

template <typename IO> NEXTWEB_INLINE bool
GenericRequest<IO>::hasHeader(std::string const &name) const {
	return mapHas(vars_, makeVarName(name));
}

template <typename IO> NEXTWEB_INLINE std::string const&
GenericRequest<IO>::getHeader(std::string const &name) const {
	return mapGet(vars_, makeVarName(name));
}

template <typename IO> NEXTWEB_INLINE typename GenericRequest<IO>::FileMap const&
GenericRequest<IO>::files() const {
	return files_;
}

template <typename IO> NEXTWEB_INLINE bool
GenericRequest<IO>::hasFile(std::string const &name) const {
	return mapHas(files_, name);
}

template <typename IO> NEXTWEB_INLINE File
GenericRequest<IO>::getFile(std::string const &name) const {
	FileMap::const_iterator i = files_.find(name);
	if (files_.end() != i) {
		return i->second;
	}
	throw Error("no file named %s", name.c_str());
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parse(std::size_t threshold) {
	char const* const* env = io_.environ();
	for (std::size_t i = 0; static_cast<char const*>(0) != env[i]; ++i) {
		parseVar(utils::makeRange(env[i]));
	}
	std::string const &method = getVar(HttpConstants::METHOD);
	if (HttpConstants::POST == method) {
		parsePost(threshold);
	}
	else if (HttpConstants::GET != method && HttpConstants::HEAD != method) {
		throw HttpError(HttpError::METHOD_NOT_ALLOWED);
	}
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parsePost(std::size_t threshold) {
	
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parseArg(utils::Range<char const*> const &range) {
	utils::Range<char const*> head, tail;
	splitOnce(range, '=', head, tail);
	args_.insert(std::make_pair(urldecode<std::string>(head), urldecode<std::string>(tail)));
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parseVar(utils::Range<char const*> const &range) {

	utils::Range<char const*> head, tail;
	utils::splitOnce(range, '=', head, tail);
	vars_.insert(std::make_pair(std::string(head.begin(), head.end()), 
		std::string(tail.begin(), tail.end())));
	if (HttpConstants::COOKIE_VAR_NAME == head) {
		parseCookies(tail);
	}
	else if (HttpConstants::QUERY_STRING_VAR_NAME == head) {
		parseQueryString(tail);
	}
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parseCookie(utils::Range<char const*> const &range) {
	utils::Range<char const*> head, tail;
	splitOnce(range, '=', head, tail);
	cookies_.insert(std::make_pair(urldecode<std::string>(head), urldecode<std::string>(tail)));
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parseCookies(utils::Range<char const*> const &range) {
	utils::Range<char const*> head, tail = range;
	while (!tail.empty()) {
		utils::splitOnce(tail, ';', head, tail);
		parseCookie(head);
		utils::trimLeft(tail);
	}
}

template <typename IO> NEXTWEB_INLINE void
GenericRequest<IO>::parseQueryString(utils::Range<char const*> const &range) {
	utils::Range<char const*> head, tail = range;
	while (!tail.empty()) {
		utils::splitFirstOfOnce(tail, "&;", head, tail);
		parseArg(head);
	}
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_GENERIC_REQUEST_HPP_INCLUDED
