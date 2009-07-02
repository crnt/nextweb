/** @file MockEnvironment.hpp */
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

#ifndef NEXTWEB_TESTS_MOCK_ENVIRONMENT_HPP_INCLUDED
#define NEXTWEB_TESTS_MOCK_ENVIRONMENT_HPP_INCLUDED

#include <list>
#include <string>
#include <cstddef>

namespace nextweb { namespace tests {

template <typename Stream>
class MockEnvironment {

public:
	MockEnvironment(Stream *stream);
	template <typename Func> MockEnvironment(Stream *stream, Func func);
	virtual ~MockEnvironment();

	MockEnvironment(MockEnvironment<Stream> const &env);
	MockEnvironment<Stream>& operator = (MockEnvironment<Stream> const &env);

	typedef std::list<char const*>::iterator iterator;
	typedef std::list<char const*>::const_iterator const_iterator;

	const_iterator end() const;
	const_iterator begin() const;

	void clear();
	void push_back(std::string const &str);

	std::size_t operator () (char *buffer, std::size_t size);

private:
	Stream *stream_;
	std::list<char const*> env_;
	std::list<std::string> strings_;
};

template <typename Stream> inline 
MockEnvironment<Stream>::MockEnvironment(Stream *stream) :
	stream_(stream)
{
}

template <typename Stream> inline 
MockEnvironment<Stream>::MockEnvironment(MockEnvironment<Stream> const &env) :
	stream_(env.stream_), env_(), strings_(env.strings_)
{
	for (std::list<std::string>::const_iterator i = strings_.begin(), end = strings_.end(); i != end; ++i) { 
		env_.push_back(i->c_str());
	}
}

template <typename Stream> inline MockEnvironment<Stream>&
MockEnvironment<Stream>::operator = (MockEnvironment<Stream> const &env) {
}

template <typename Stream> template <typename Func> inline
MockEnvironment<Stream>::MockEnvironment(Stream *stream, Func func) :
	stream_(stream), env_(), strings_()
{
	func(stream_);
}

template <typename Stream> inline 
MockEnvironment<Stream>::~MockEnvironment() {
}

template <typename Stream> inline typename MockEnvironment<Stream>::const_iterator
MockEnvironment<Stream>::end() const {
	return env_.end();
}

template <typename Stream> inline typename MockEnvironment<Stream>::const_iterator
MockEnvironment<Stream>::begin() const {
	return env_.begin();
}

template <typename Stream> inline void
MockEnvironment<Stream>::clear() {
	env_.clear();
	strings_.clear();
}

template <typename Stream> inline void
MockEnvironment<Stream>::push_back(std::string const &str) {
	strings_.push_back(str);
	std::string const &value = strings_.back();
	env_.push_back(value.c_str());
}

template <typename Stream> inline std::size_t
MockEnvironment<Stream>::operator () (char *buffer, std::size_t size) {
	stream_->read(buffer, size);
	return stream_->gcount();
}

}} // namespaces

#endif // NEXTWEB_TESTS_MOCK_ENVIRONMENT_HPP_INCLUDED
