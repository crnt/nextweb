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

#ifndef NEXTWEB_FASTCGI_POST_PARSER_HPP_INCLUDED
#define NEXTWEB_FASTCGI_POST_PARSER_HPP_INCLUDED

#include <string>

#include "nextweb/Config.hpp"
#include "nextweb/Shared.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/StringUtils.hpp"
#include "nextweb/utils/StringConverters.hpp"

#include "nextweb/fastcgi/File.hpp"
#include "nextweb/fastcgi/HttpError.hpp"
#include "nextweb/fastcgi/impl/HttpUtils.hpp"
#include "nextweb/fastcgi/impl/UrlEncode.hpp"
#include "nextweb/fastcgi/impl/IterFileImpl.hpp"

namespace nextweb { namespace fastcgi {

class PostParserListener {

public:
	PostParserListener();
	virtual ~PostParserListener();

	virtual std::string const& contentType() const = 0;
	virtual void addFile(std::string const &name, File const &file) = 0;
	virtual void addArg(std::string const &name, std::string const &value) = 0;
	
private:
	PostParserListener(PostParserListener const &);
	PostParserListener& operator = (PostParserListener const &);
};

template <typename IO>
class PostParser : public Shared {

public:
	PostParser(PostParserListener *listener);
	virtual ~PostParser();
	
	virtual void clear() = 0;
	virtual std::size_t available() const = 0;
	virtual void parsePost(IO &io, std::size_t size) = 0;

protected:
	std::string const& contentType() const;
	void fireAddFile(std::string const &name, File const &file);
	void fireAddArg(std::string const &name, std::string const &value);

private:
	PostParser(PostParser const &);
	PostParser& operator = (PostParser const &);

private:
	PostParserListener *listener_;
};

template <typename IO, typename Container>
class ContainerPostParser : public PostParser<IO> {

public:
	ContainerPostParser(PostParserListener *listener);
	virtual ~ContainerPostParser();
	
	virtual void clear();
	virtual std::size_t available() const;
	virtual void parsePost(IO &io, std::size_t size);

private:
	ContainerPostParser(ContainerPostParser const &);
	ContainerPostParser& operator = (ContainerPostParser const &);
	
	typedef utils::Range<std::string::const_iterator> StringRangeType;
	typedef utils::Range<typename Container::const_iterator> RangeType;
	
	void parsePlainPost();
	void parseArg(RangeType const &part);
	void parsePart(RangeType const &part);
	void parseMultipart(std::string const &bound);
	std::string getBoundary(StringRangeType const &value) const;

private:
	Container container_;	
};

template <typename IO> NEXTWEB_INLINE
PostParser<IO>::PostParser(PostParserListener *listener) :
	listener_(listener)
{
}

template <typename IO> NEXTWEB_INLINE 
PostParser<IO>::~PostParser() {
}

template <typename IO> NEXTWEB_INLINE std::string const&
PostParser<IO>::contentType() const {
	return listener_->contentType();
}

template <typename IO> NEXTWEB_INLINE void
PostParser<IO>::fireAddFile(std::string const &name, File const &file) {
	listener_->addFile(name, file);
}

template <typename IO> NEXTWEB_INLINE void
PostParser<IO>::fireAddArg(std::string const &name, std::string const &value) {
	listener_->addArg(name, value);
}

template <typename IO, typename Container> NEXTWEB_INLINE
ContainerPostParser<IO, Container>::ContainerPostParser(PostParserListener *listener) :
	PostParser<IO>(listener), container_()
{
}

template <typename IO, typename Container> NEXTWEB_INLINE
ContainerPostParser<IO, Container>::~ContainerPostParser() {
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::clear() {
	container_.clear();
}

template <typename IO, typename Container> NEXTWEB_INLINE std::size_t
ContainerPostParser<IO, Container>::available() const {
	return container_.size();
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parsePost(IO &io, std::size_t size) {
	
	using namespace utils;
	container_.resize(size);
	io.read(&container_[0], size);
	std::string const &type = this->contentType();

	StringRangeType range(type.begin(), type.end());
	range = trim(range);
	if (startsWith(range, HttpConstants::MULTIPART_FORM_DATA)) {
		StringRangeType head, tail;
		splitOnce(range, ';', head, tail);
		parseMultipart(getBoundary(trim(tail)));
	}
	else {
		parsePlainPost();
	}
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parsePlainPost() {
	using namespace utils;
	RangeType range(container_.begin(), container_.end()), part;
	while (!range.empty()) {
		splitFirstOfOnce(range, "&;", part, range);
		parseArg(part);
	}
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parseArg(typename ContainerPostParser<IO, Container>::RangeType const &part) {
	RangeType head, tail;
	utils::splitOnce(part, '=', head, tail);
	this->fireAddArg(urlencode<std::string>(head), urlencode<std::string>(tail));
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parsePart(typename ContainerPostParser<IO, Container>::RangeType const &part) {
	
	using namespace utils;
	
	typedef typename Container::value_type CharType;
	typedef typename Container::const_iterator IteratorType;
	
	IsLineEnd<CharType> checker;
	typename RangeType::const_iterator begin = part.begin(), end = part.end();
	typename RangeType::const_iterator lineEnd = nextMatched(begin, end, checker);
	typename RangeType::const_iterator newLine = nextNotMatched(lineEnd, end, checker);

	RangeType token(lineEnd, newLine);
	if (HttpConstants::RNRN != token && HttpConstants::NN != token) {
		throw HttpError(HttpError::BAD_REQUEST);
	}
	RangeType name, filename, type;
	RangeType header(begin, lineEnd), content(newLine, end);
	
	if (filename.empty()) {
		fireAddArg(utils::toString(name), utils::toString(content));
	}
	else {
		SharedPtr<FileImpl> impl(new IterFileImpl<IteratorType>(filename, type, content));
		this->fireAddFile(utils::toString(name), File(impl));
	}
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parseMultipart(std::string const &bound) {
	using namespace utils;
	typedef Range<typename Container::const_iterator> RangeType;
	RangeType range(container_.begin(), container_.end()), part;
	while (!range.empty()) {
		splitOnce(range, bound, part, range);
		parsePart(part);
	}
}

template <typename IO, typename Container> NEXTWEB_INLINE std::string
ContainerPostParser<IO, Container>::getBoundary(typename ContainerPostParser<IO, Container>::StringRangeType const &value) const {

	using namespace utils;
	if (!startsWith(value, HttpConstants::BOUNDARY)) {
		throw HttpError(HttpError::BAD_REQUEST);
	}

	std::string result("--");
	StringRangeType head, tail;
	splitOnce(value, '=', head, tail);
	tail = trim(tail);
	if ((tail.size() < static_cast<StringRangeType::size_type>(2)) || '"' != *(tail.begin()) || '"' != *(tail.rbegin())) {
		throw HttpError(HttpError::BAD_REQUEST);
	}

	tail = truncate(tail, 1, 1);
	result.append(tail.begin(), tail.end());
	return result;
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_POST_PARSER_HPP_INCLUDED
