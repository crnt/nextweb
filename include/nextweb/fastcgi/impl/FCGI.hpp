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

#ifndef NEXTWEB_FASTCGI_FCGI_HPP_INCLUDED
#define NEXTWEB_FASTCGI_FCGI_HPP_INCLUDED

#include <cstddef>
#include <fcgiapp.h>

namespace nextweb { namespace fastcgi {

class FCGI {

public:
	FCGI(int sock);
	virtual ~FCGI();

	void accept();
	void finish();

	std::size_t read(char *buffer, std::size_t size); 
	std::size_t write(char const *buffer, std::size_t size);

private:
	FCGI(FCGI const &);
	FCGI& operator = (FCGI const &);

	void setFlag(int flag);
	void checkFlag(int flag) const;
	bool isFlagSet(int flag) const;

	static int const REQUEST_INITIALIZED = 1;
	static int const REQUEST_ACCEPTED = 2;
	static int const REQUEST_FINISHED = 4;

private:
	int flags_;
	FCGX_Request request_;
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_FCGI_HPP_INCLUDED
