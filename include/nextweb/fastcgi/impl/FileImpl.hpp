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

#ifndef NEXTWEB_FASTCGI_FILE_IMPL_HPP_INCLUDED
#define NEXTWEB_FASTCGI_FILE_IMPL_HPP_INCLUDED

#include <string>
#include <iosfwd>

#include "nextweb/Shared.hpp"

namespace nextweb { namespace fastcgi {

class FileImpl : public Shared {

public:
	FileImpl();
	virtual ~FileImpl();

	virtual std::streamsize size() const = 0;
	virtual std::string const& name() const = 0;
	virtual std::string const& contentType() const = 0;

	virtual std::istream& stream() = 0;
	virtual void save(char const *name) const = 0;

private:
	FileImpl(FileImpl const &);
	FileImpl& operator = (FileImpl const &);
};

}} // namespaces

#endif // NEXTWEB_FASTCGI_FILE_IMPL_HPP_INCLUDED
