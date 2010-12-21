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

#ifndef NEXTWEB_TESTS_MOCK_IO_HPP_INCLUDED
#define NEXTWEB_TESTS_MOCK_IO_HPP_INCLUDED

#include <string>
#include <vector>
#include <fstream>

namespace nextweb { namespace tests {

class MockIO {

public:
	MockIO();
	virtual ~MockIO();

	void checkIsValid() const;
	void add(std::string const &value);
	void attachFile(char const *fileName);

	char const* const* environ() const; 
	std::size_t read(char *buffer, std::size_t size); 
	std::size_t write(char const *buffer, std::size_t size);
	
private:
	MockIO(MockIO const &);
	MockIO& operator = (MockIO const &);
	void validate() const;

private:
	std::ifstream post_;
	std::vector<std::string> strings_;
	mutable std::vector<char const*> env_;
};

}} // namespaces

#endif // NEXTWEB_TESTS_MOCK_IO_HPP_INCLUDED
