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

#ifndef NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED
#define NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED

#include "nextweb/Config.hpp"
#include "nextweb/Shared.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/utils/Resource.hpp"

namespace nextweb { namespace utils {

struct DescriptorTraits {
	void destroy(int fdes);
	static int defaultValue();
};

class MappedFileImpl : public Shared {

public:
	typedef off_t SizeType;

	MappedFileImpl(char const *name, bool readOnly);
	virtual ~MappedFileImpl();

	SizeType size() const;
	void resize(SizeType newsize);
	
private:
	MappedFileImpl(MappedFileImpl const &);
	MappedFileImpl& operator = (MappedFileImpl const &);

private:
	SizeType size_;
	Resource<int, DescriptorTraits> fdes_;
};

template <typename Type>
class ReadableMappedFile {

public:
	typedef Type value_type;
	typedef off_t size_type;
	
	ReadableMappedFile(char const *name);
	virtual ~ReadableMappedFile();
	
private:
	SharedPtr<MappedFileImpl> impl_;
};

template <typename Type>
class WriteableMappedFile {
};

}} // namespaces

#endif // NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED
