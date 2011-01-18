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

#ifndef NEXTWEB_FASTCGI_FAST_CGI_IO_HPP_INCLUDED
#define NEXTWEB_FASTCGI_FAST_CGI_IO_HPP_INCLUDED

#include <string>
#include <cstddef>
#include <fcgiapp.h>

#include "nextweb/Config.hpp"

namespace nextweb { namespace fastcgi {

class FastCgiIO {

public:
	FastCgiIO(int socket);
	virtual ~FastCgiIO();

	void accept();
	void finish();

	char const* const* environ() const;
	std::size_t read(char *buffer, std::size_t size);
	
	void setStatus(unsigned short status);
	std::size_t write(char const *buffer, std::size_t size);
	std::size_t writeHeader(std::string const &name, std::string const &value);
	
	template <typename Request> void setup(Request const &request);

private:
	FastCgiIO(FastCgiIO const &);
	FastCgiIO& operator = (FastCgiIO const &);

private:
	FCGX_Request request_;
};

template <typename Request> NEXTWEB_INLINE void
FastCgiIO::setup(Request const &request) {
}

}} // namespaces

#endif // NEXTWEB_FASTCGI_FAST_CGI_IO_HPP_INCLUDED
