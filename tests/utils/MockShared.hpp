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

#ifndef NEXTWEB_TESTS_MOCK_SHARED_HPP_INCLUDED
#define NEXTWEB_TESTS_MOCK_SHARED_HPP_INCLUDED

#include <set>

#include "nextweb/Shared.hpp"

namespace nextweb { namespace tests {

class MockShared : public Shared {

public:
	MockShared();
	virtual ~MockShared();
	static bool isLive(MockShared *object);

private:
	MockShared(MockShared const &);
	MockShared& operator = (MockShared const &);

private:
	static std::set<MockShared*> objects_;
};

class MockSharedDerived : public MockShared {

public:
	MockSharedDerived();
	virtual ~MockSharedDerived();

private:
	MockSharedDerived(MockSharedDerived const &);
	MockSharedDerived& operator = (MockSharedDerived const &);
};

}} // namespaces

#endif // NEXTWEB_TESTS_MOCK_SHARED_HPP_INCLUDED
