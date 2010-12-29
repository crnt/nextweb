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

#ifndef NEXTWEB_FASTCGI_FILE_HPP_INCLUDED
#define NEXTWEB_FASTCGI_FILE_HPP_INCLUDED

#include <string>
#include <iosfwd>

#include "nextweb/Config.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/fastcgi/Forward.hpp"

namespace nextweb { namespace fastcgi {

class NEXTWEB_API File {

public:
	File();
	virtual ~File();
	explicit File(SharedPtr<FileImpl> const &impl);

	File(File const &other);
	File& operator = (File const &other);

	std::streamsize size() const;
	std::string const& name() const;
	std::string const& contentType() const;

	std::istream& stream();
	void save(char const *name) const;
	void save(std::string const &name) const;

	operator std::istream& ();

private:
	SharedPtr<FileImpl> impl_;
};

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/fastcgi/File.hpp"
#endif

#endif // NEXTWEB_FASTCGI_FILE_HPP_INCLUDED
