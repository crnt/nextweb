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
#include "nextweb/utils/Resource.hpp"
#include "nextweb/fastcgi/File.hpp"

namespace nextweb { namespace fastcgi {

class PostParserListener {

public:
	PostParserListener();
	virtual ~PostParserListener();

	virtual void addFile(File const &file) = 0;
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
	void fireAddFile(File const &file);
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

template <typename IO> NEXTWEB_INLINE void
PostParser<IO>::fireAddFile(File const &file) {
	listener_->addFile(file);
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
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_POST_PARSER_HPP_INCLUDED
