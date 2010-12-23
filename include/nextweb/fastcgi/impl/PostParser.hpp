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
	
	using PostParser<IO>::contentType;
	using PostParser<IO>::fireAddFile;
	using PostParser<IO>::fireAddArg;

	typedef utils::Range<std::string::const_iterator> StringRangeType;
	typedef utils::Range<typename Container::iterator> RangeType;
	
	void parsePlainPost();
	void parseArg(RangeType const &arg);
	void parsePart(RangeType const &part);
	void parseMultipart(std::string const &bound);
	void processPart(RangeType const &header, RangeType const &content);
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
	std::string const &type = contentType();

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
	fireAddArg(urlencode<std::string>(head), urlencode<std::string>(tail));
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parsePart(typename ContainerPostParser<IO, Container>::RangeType const &part) {
	
	using namespace utils;
	IsLineEnd<typename Container::value_type> checker;
	
	typename RangeType::iterator const begin = part.begin(), end = part.end();
	typename RangeType::iterator newLine = begin, lineEnd;

	for (std::size_t count = 0; true; ++count) {
		lineEnd = nextMatched(newLine, end, checker);
 		newLine = nextNotMatched(lineEnd, end, checker);
		RangeType token(lineEnd, newLine);

		if (token.empty()) {
			throw HttpError(HttpError::BAD_REQUEST);
		}
		else if (HttpConstants::RNRN == token || HttpConstants::NN == token) {
			break;
		}
	}
	RangeType header(begin, lineEnd), content(newLine, end);
	processPart(header, content);
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::parseMultipart(std::string const &bound) {
	using namespace utils;
	RangeType range(container_.begin(), container_.end()), part;
	while (!range.empty()) {
		splitOnce(range, bound, part, range);
		if (!part.empty()) {
			parsePart(part);
		}
	}
}

template <typename IO, typename Container> NEXTWEB_INLINE void
ContainerPostParser<IO, Container>::processPart(typename ContainerPostParser<IO, Container>::RangeType const &header, typename ContainerPostParser<IO, Container>::RangeType const &content) {
	
	RangeType name, filename, type;
	typedef typename Container::iterator IteratorType;
	if (filename.empty()) {
		fireAddArg(utils::toString(name), utils::toString(content));
	}
	else {
		SharedPtr<FileImpl> impl(new IterFileImpl<IteratorType>(filename, type, content));
		fireAddFile(utils::toString(name), File(impl));
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
