/** @file RequestParams.hpp */
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

#ifndef NEXTWEB_DETAILS_REQUEST_PARAMS_HPP_INCUDED
#define NEXTWEB_DETAILS_REQUEST_PARAMS_HPP_INCUDED

#include <map>
#include <cstdio>
#include <cstddef>

#include "nextweb/Error.hpp"

#include "details/MapUtils.hpp"
#include "details/UrlEncode.hpp"
#include "details/StringUtils.hpp"
#include "details/HttpConstants.hpp"
#include "details/WriteableFile.hpp"
#include "details/EnumerationImpl.hpp"
#include "details/StringConverters.hpp"

namespace nextweb { namespace details {

template <typename Setup>
class RequestParams {

public:
	RequestParams();
	virtual ~RequestParams();

	typedef typename Setup::StringType StringType;
	typedef std::pair<StringType const, StringType> ParamType;
	
	bool hasArg(StringType const &name) const;
	StringType const& getArg(StringType const &name) const;
	typename Enumeration<ParamType const&>::PtrType args() const;
	typename Enumeration<StringType const&>::PtrType argNames() const;
	typename Enumeration<StringType const&>::PtrType argList(StringType const &name) const;
	
	bool hasVar(StringType const &name) const;
	StringType const& getVar(StringType const &name) const;
	typename Enumeration<ParamType const&>::PtrType vars() const;
	typename Enumeration<StringType const&>::PtrType varNames() const;
	
	bool hasCookie(StringType const &name) const;
	StringType const& getCookie(StringType const &name) const;
	typename Enumeration<ParamType const&>::PtrType cookies() const;
	typename Enumeration<StringType const&>::PtrType cookieNames() const;

	bool hasHttp(StringType const &name) const;
	StringType const& getHttp(StringType const &name) const;
	
	template <typename Context> void parse(Context const &cont, off_t threshold);

private:
	RequestParams(RequestParams const &);
	RequestParams& operator = (RequestParams const &);

	typedef typename Setup::template Allocator<ParamType>::Type ParamAllocatorType;
	typedef std::map<StringType, StringType, std::less<StringType>, ParamAllocatorType> VarMapType;
	typedef std::multimap<StringType, StringType, std::less<StringType>, ParamAllocatorType> ArgMapType;
	
	void parseVar(char const *var);
	void parseCookies(StringType const &cookies);
	void parseSingleCookie(Range<typename StringType::const_iterator> const &cookie);
	
	template <typename Iter> void parsePost(Range<Iter> const &range);
	template <typename Iter> void parseUrlEncoded(Range<Iter> const &range);
	template <typename Iter> void parseSingleArgument(Range<Iter> const &arg);
	template <typename Context> void parseRequest(Context const &ctx, off_t threshold);
	
	off_t getContentLength() const;
	StringType makeHeaderName(StringType const &name) const;
	
	static StringType const EMPTY_STRING;
	
private:
	ArgMapType args_;
	VarMapType vars_, cookies_;
	typename Setup::template Allocator<char>::Type alloc_;
};

template <typename Setup> typename RequestParams<Setup>::StringType const
RequestParams<Setup>::EMPTY_STRING;

template <typename Setup> inline
RequestParams<Setup>::RequestParams()
{
}

template <typename Setup> inline
RequestParams<Setup>::~RequestParams() {
}

template <typename Setup> inline bool
RequestParams<Setup>::hasArg(typename RequestParams<Setup>::StringType const &name) const {
	return has(args_, name);
}

template <typename Setup> inline typename RequestParams<Setup>::StringType const&
RequestParams<Setup>::getArg(typename RequestParams<Setup>::StringType const &name) const {
	return get(args_, name, EMPTY_STRING);
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::ParamType const&>::PtrType
RequestParams<Setup>::args() const {
	return makeEnumeration(args_);
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::StringType const&>::PtrType
RequestParams<Setup>::argNames() const {
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::StringType const&>::PtrType
RequestParams<Setup>::argList(typename RequestParams<Setup>::StringType const &name) const {
}

template <typename Setup> inline bool 
RequestParams<Setup>::hasVar(RequestParams<Setup>::StringType const &name) const {
	return has(vars_, name);
}

template <typename Setup> inline typename RequestParams<Setup>::StringType const&
RequestParams<Setup>::getVar(typename RequestParams<Setup>::StringType const &name) const {
	return get(vars_, name, EMPTY_STRING);
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::ParamType const&>::PtrType
RequestParams<Setup>::vars() const {
	return makeEnumeration(vars_);
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::StringType const&>::PtrType
RequestParams<Setup>::varNames() const {
	return makeMapKeyEnumeration(vars_);
}

template <typename Setup> inline bool
RequestParams<Setup>::hasCookie(typename RequestParams<Setup>::StringType const &name) const {
	return has(cookies_, name);
}

template <typename Setup> inline typename RequestParams<Setup>::StringType const&
RequestParams<Setup>::getCookie(RequestParams<Setup>::StringType const &name) const {
	return get(cookies_, name, EMPTY_STRING);
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::ParamType const&>::PtrType
RequestParams<Setup>::cookies() const {
	return makeEnumeration(cookies_);
}

template <typename Setup> inline typename Enumeration<typename RequestParams<Setup>::StringType const&>::PtrType
RequestParams<Setup>::cookieNames() const {
	return makeMapKeyEnumeration(cookies_);
}

template <typename Setup> inline bool
RequestParams<Setup>::hasHttp(typename RequestParams<Setup>::StringType const &name) const {
	return hasVar(makeHeaderName(name));
}

template <typename Setup> inline typename RequestParams<Setup>::StringType const&
RequestParams<Setup>::getHttp(typename RequestParams<Setup>::StringType const &name) const {
	return getVar(makeHeaderName(name));
}

template <typename Setup> template <typename Context> inline void
RequestParams<Setup>::parse(Context const &ctx, off_t threshold) {
	for (typename Context::const_iterator i = ctx.begin(), end = ctx.end(); i != end; ++i) {
		parseVar(*i);
	}
	parseRequest(ctx, threshold);
}

template <typename Setup> inline void
RequestParams<Setup>::parseVar(char const *var) {
	Range<char const*> head, tail;
	split(makeRange(var), '=', head, tail);
	Range<char const*> key = trim(head), value = trim(tail);
	vars_[StringType(key.begin(), key.end())] = StringType(value.begin(), value.end());
}

template <typename Setup> template <typename Context> inline void
RequestParams<Setup>::parseRequest(Context const &ctx, off_t threshold) {
	StringType const &cookie = getVar(HttpConstants<Setup>::COOKIE);
	parseCookies(cookie);
	StringType const &method = getVar(HttpConstants<Setup>::REQUEST_METHOD);
	if (method == HttpConstants<Setup>::POST) {
		off_t size = getContentLength();
		if (size > threshold) {
			WriteableFile<char> file = WriteableFile<char>::temporary(threshold);
			file.fill(ctx, size);
			parsePost(makeRange(file.begin(), file.end()));
		} 
		else {
			
		}
	}
	else {
		StringType const &query = getVar(HttpConstants<Setup>::QUERY_STRING);
		parseUrlEncoded(makeRange(query.begin(), query.end()));
	}
}

template <typename Setup> inline void
RequestParams<Setup>::parseCookies(typename RequestParams<Setup>::StringType const &cookie) {
	typedef Range<typename StringType::const_iterator> RangeType;
	RangeType copy = RangeType(cookie.begin(), cookie.end()), head, tail;
	while (!copy.empty()) {
		split(copy, HttpConstants<Setup>::COOKIE_DELIMITER, head, tail);
		parseSingleCookie(trim(head));
		copy = trim(tail);
	}
}

template <typename Setup> inline void
RequestParams<Setup>::parseSingleCookie(Range<typename RequestParams<Setup>::StringType::const_iterator> const &cookie) {
	Range<typename RequestParams<Setup>::StringType::const_iterator> value, key;
	split(cookie, '=', key, value);
	if (!key.empty()) {
		cookies_[urldecode<StringType>(key)] = urldecode<StringType>(value);
	}
}

template <typename Setup> template <typename Iter> inline void
RequestParams<Setup>::parsePost(Range<Iter> const &range) {
}

template <typename Setup> template <typename Iter> inline void
RequestParams<Setup>::parseUrlEncoded(Range<Iter> const &range) {
	Range<Iter> copy = range , head, tail;
	while (!copy.empty()) {
		splitFirstOf(copy, ";&", head, tail);
		parseSingleArgument(head);
		copy = tail;
	}
}

template <typename Setup> template <typename Iter> inline void
RequestParams<Setup>::parseSingleArgument(Range<Iter> const &arg) {
	Range<Iter> key, value;
	split(arg, '=', key, value);
	if (!key.empty()) {
		args_.insert(std::make_pair(urldecode<StringType>(key), urldecode<StringType>(value)));
	}
}

template <typename Setup> inline off_t
RequestParams<Setup>::getContentLength() const {
	return fromString<Setup, off_t>(getVar(HttpConstants<Setup>::CONTENT_LENGTH));
}

template <typename Setup> inline typename RequestParams<Setup>::StringType
RequestParams<Setup>::makeHeaderName(typename RequestParams<Setup>::StringType const &name) const {
	StringType header = toUpper(name);
	std::replace(header.begin(), header.end(), '-', '_');
	if (startsWith(header, HttpConstants<Setup>::HTTP_PREFIX)) {
		return header;
	}
	return StringType("HTTP_").append(header);
}

}} // namespace

#endif // NEXTWEB_DETAILS_REQUEST_PARAMS_HPP_INCUDED
