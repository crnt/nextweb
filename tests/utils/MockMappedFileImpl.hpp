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

#ifndef NEXTWEB_TESTS_MOCK_MAPPED_FILE_IMPL_HPP_INCLUDED
#define NEXTWEB_TESTS_MOCK_MAPPED_FILE_IMPL_HPP_INCLUDED

#include <map>
#include <cassert>
#include <cstddef>

#include "nextweb/Error.hpp"
#include "nextweb/Config.hpp"

namespace nextweb { namespace tests {

template <typename Impl>
class MockMappedFileImpl : public Impl {

public:
	typedef MockMappedFileImpl<Impl> Type;
	typedef typename Impl::SizeType SizeType;

	MockMappedFileImpl(char const *name, bool readOnly, SizeType pageSize);
	
	void reservePage(SizeType pos);
	void releasePage(SizeType pos) throw ();

	bool pagesExists() const;
	std::size_t pageCounter(SizeType pos) const;

private:
	std::map<SizeType, int> pages_;
};

template <typename Impl> NEXTWEB_INLINE
MockMappedFileImpl<Impl>::MockMappedFileImpl(char const *name, bool readOnly, typename MockMappedFileImpl<Impl>::SizeType pageSize) :
	Impl(name, readOnly, pageSize)
{
}
	
template <typename Impl> NEXTWEB_INLINE void
MockMappedFileImpl<Impl>::reservePage(typename MockMappedFileImpl<Impl>::SizeType pos) {
	
	SizeType pageNum = pos / this->pageSize();
	SizeType realPos = pageNum * this->pageSize();
	
	typename std::map<SizeType, int>::iterator i = pages_.find(realPos);
	if (pages_.end() == i) {
		pages_.insert(std::pair<SizeType, std::size_t>(realPos, 1));
	}
	else {
		++(i->second);
	}
	Impl::reservePage(realPos);
}

template <typename Impl> NEXTWEB_INLINE void
MockMappedFileImpl<Impl>::releasePage(typename MockMappedFileImpl<Impl>::SizeType pos) throw () {
	
	SizeType pageNum = pos / this->pageSize();
	SizeType realPos = pageNum * this->pageSize();
	
	typename std::map<SizeType, int>::iterator i = pages_.find(realPos);
	assert(pages_.end() != i && i->second >= 0);
	if (0 == --(i->second)) {
		pages_.erase(i);
	}

	Impl::releasePage(realPos);
}

template <typename Impl> NEXTWEB_INLINE bool
MockMappedFileImpl<Impl>::pagesExists() const {
	return !pages_.empty();
}

template <typename Impl> NEXTWEB_INLINE std::size_t
MockMappedFileImpl<Impl>::pageCounter(SizeType pos) const {
	typename std::map<SizeType, int>::const_iterator i = pages_.find(pos);
	return (pages_.end() == i) ? 0 : i->second;
}

}} // namespaces

#endif // NEXTWEB_TESTS_MOCK_MAPPED_FILE_IMPL_HPP_INCLUDED
