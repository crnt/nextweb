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
#include "nextweb/fastcgi/impl/LineEndFilter.hpp"

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

template <typename IO, typename Sequence>
class SequencePostParser : public PostParser<IO> {

public:
	SequencePostParser(PostParserListener *listener);
	virtual ~SequencePostParser();
	
	virtual void clear();
	virtual std::size_t available() const;
	virtual void parsePost(IO &io, std::size_t size);

private:
	SequencePostParser(SequencePostParser const &);
	SequencePostParser& operator = (SequencePostParser const &);
	
	using PostParser<IO>::contentType;
	using PostParser<IO>::fireAddFile;
	using PostParser<IO>::fireAddArg;

	typedef utils::Range<typename Sequence::iterator> RangeType;
	typedef utils::Range<std::string::const_iterator> StringRangeType;
	
	void parsePlainPost();
	void parseArg(RangeType const &arg);
	void parsePart(RangeType const &part);
	void parseMultipart(std::string const &bound);
	void processPart(RangeType const &header, RangeType const &content);
	
	std::string getBoundary(StringRangeType const &value) const;
	StringRangeType stripBoundaryQuotes(StringRangeType const &value) const;

private:
	Sequence container_;	
};

template <typename Sequence>
class MultipartHeaderParser {

public:
	MultipartHeaderParser(Sequence const &seq);
	
	typedef typename Sequence::iterator IteratorType;
	typedef LineEndFilter<IteratorType> FilteredIteratorType;
	typedef utils::Range<FilteredIteratorType> FilteredType;
	
	
	bool isFile() const;
	FilteredType name() const;
	FilteredType fileName() const;
	FilteredType contentType() const;

private:
	MultipartHeaderParser(MultipartHeaderParser const &);
	MultipartHeaderParser& operator = (MultipartHeaderParser const &);

	typedef typename Sequence::value_type CharType;

	void parse(FilteredType const &filtered);
	void parseLine(FilteredType const &filtered);
	void parseContentDisposition(FilteredType const &filtered);

private:
	FilteredType name_, type_, fileName_;
	utils::IsSpace<CharType> spaceChecker_;
	utils::IsLineEnd<CharType> lineEndChecker_;
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

template <typename IO, typename Sequence> NEXTWEB_INLINE
SequencePostParser<IO, Sequence>::SequencePostParser(PostParserListener *listener) :
	PostParser<IO>(listener), container_()
{
}

template <typename IO, typename Sequence> NEXTWEB_INLINE
SequencePostParser<IO, Sequence>::~SequencePostParser() {
}

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::clear() {
	container_.clear();
}

template <typename IO, typename Sequence> NEXTWEB_INLINE std::size_t
SequencePostParser<IO, Sequence>::available() const {
	return container_.size();
}

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::parsePost(IO &io, std::size_t size) {
	
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

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::parsePlainPost() {
	using namespace utils;
	RangeType range(container_.begin(), container_.end()), part;
	while (!range.empty()) {
		splitFirstOfOnce(range, "&;", part, range);
		parseArg(part);
	}
}

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::parseArg(typename SequencePostParser<IO, Sequence>::RangeType const &part) {
	RangeType head, tail;
	utils::splitOnce(part, '=', head, tail);
	fireAddArg(urlencode<std::string>(head), urlencode<std::string>(tail));
}

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::parsePart(typename SequencePostParser<IO, Sequence>::RangeType const &part) {
	
	using namespace utils;
	IsLineEnd<typename Sequence::value_type> checker;
	
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

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::parseMultipart(std::string const &bound) {

	using namespace utils;
	
	bool endingFound = false;
	IsLineEnd<typename RangeType::value_type> lineEnd;
	RangeType range(container_.begin(), container_.end()), part;
	while (!range.empty()) {
		splitOnce(range, bound, part, range);
		part = trimIf(part, lineEnd);
		if (HttpConstants::MULTIPART_ENDING == part) {
			endingFound = true;
		}
		else if (!part.empty()) {
			parsePart(part);
		}
	}
	if (!endingFound) {
		throw HttpError(HttpError::BAD_REQUEST);
	}
}

template <typename IO, typename Sequence> NEXTWEB_INLINE void
SequencePostParser<IO, Sequence>::processPart(typename SequencePostParser<IO, Sequence>::RangeType const &header, typename SequencePostParser<IO, Sequence>::RangeType const &content) {
	
	MultipartHeaderParser<RangeType> parser(header);
	typedef typename RangeType::iterator IteratorType;
	if (parser.isFile()) {
		SharedPtr<FileImpl> impl(new IterFileImpl<IteratorType>(utils::toString(parser.fileName()), utils::toString(parser.contentType()), content));
		fireAddFile(utils::toString(parser.name()), File(impl));
	}
	else {
		fireAddArg(utils::toString(parser.name()), utils::toString(content));
	}
}

template <typename IO, typename Sequence> NEXTWEB_INLINE std::string
SequencePostParser<IO, Sequence>::getBoundary(typename SequencePostParser<IO, Sequence>::StringRangeType const &value) const {

	using namespace utils;
	if (!startsWith(value, HttpConstants::BOUNDARY)) {
		throw HttpError(HttpError::BAD_REQUEST);
	}

	std::string result("--");
	StringRangeType head, tail;
	splitOnce(value, '=', head, tail);
	tail = stripBoundaryQuotes(trim(tail));
	result.append(tail.begin(), tail.end());
	return result;
}

template <typename IO, typename Sequence> NEXTWEB_INLINE typename SequencePostParser<IO, Sequence>::StringRangeType
SequencePostParser<IO, Sequence>::stripBoundaryQuotes(typename SequencePostParser<IO, Sequence>::StringRangeType const &value) const {
	if ((value.size() < static_cast<typename StringRangeType::size_type>(2)) || '"' != *(value.begin())) {
		return value;
	}
	if ('"' == *(value.rbegin())) {
		return trimChars(value, '"');
	}
	throw HttpError(HttpError::BAD_REQUEST);
}

template <typename Sequence> NEXTWEB_INLINE
MultipartHeaderParser<Sequence>::MultipartHeaderParser(Sequence const &seq) :
	name_(), type_(), fileName_()
{
	parse(makeLineEndFiltered(seq.begin(), seq.end()));
}

template <typename Sequence> NEXTWEB_INLINE bool
MultipartHeaderParser<Sequence>::isFile() const {
	return !fileName_.empty();
}

template <typename Sequence> NEXTWEB_INLINE typename MultipartHeaderParser<Sequence>::FilteredType
MultipartHeaderParser<Sequence>::name() const {
	return name_;
}

template <typename Sequence> NEXTWEB_INLINE typename MultipartHeaderParser<Sequence>::FilteredType
MultipartHeaderParser<Sequence>::fileName() const {
	return fileName_;
}

template <typename Sequence> NEXTWEB_INLINE typename MultipartHeaderParser<Sequence>::FilteredType
MultipartHeaderParser<Sequence>::contentType() const {
	return type_;
}

template <typename Sequence> NEXTWEB_INLINE void
MultipartHeaderParser<Sequence>::parse(typename MultipartHeaderParser<Sequence>::FilteredType const &filtered) {
	
	using namespace utils;
	
	FilteredType range = filtered;
	IsLineEnd<typename FilteredType::value_type> checker;
	
	while (!range.empty()) {
		typename FilteredType::iterator begin = range.begin(), end = range.end();
		typename FilteredType::iterator lineEnd = nextMatched(begin, end, checker);
		typename FilteredType::iterator newLine = nextNotMatched(lineEnd, end, checker);
		parseLine(makeRange(begin, lineEnd));
		range = makeRange(newLine, end);
	}
}

template <typename Sequence> NEXTWEB_INLINE void
MultipartHeaderParser<Sequence>::parseLine(typename MultipartHeaderParser<Sequence>::FilteredType const &filtered) {
	
	using namespace utils;
	
	FilteredType head, tail;
	splitOnce(filtered, ':', head, tail);
	tail = trim(tail);
	if (utils::isCIEqual(HttpConstants::CONTENT_DISPOSITION, head)) {
		parseContentDisposition(tail);
	}
	else if (isCIEqual(HttpConstants::CONTENT_TYPE, head)) {
		type_ = tail;
	}
}

template <typename Sequence> NEXTWEB_INLINE void
MultipartHeaderParser<Sequence>::parseContentDisposition(typename MultipartHeaderParser<Sequence>::FilteredType const &filtered) {
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_POST_PARSER_HPP_INCLUDED
