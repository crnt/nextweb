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

#ifndef NEXTWEB_UTILS_TYPE_TRAITS_HPP_INCLUDED
#define NEXTWEB_UTILS_TYPE_TRAITS_HPP_INCLUDED

namespace nextweb { namespace utils {

struct TrueType {
	static bool const RESULT = true;
};

struct FalseType {
	static bool const RESULT = false;
};

template <typename Arg, typename Other>
struct IsSame : public FalseType {
};

template <typename Arg>
struct IsSame<Arg, Arg> : public TrueType {
};

template <typename Arg>
struct IsConst : public FalseType {
};

template <typename Arg>
struct IsConst<Arg const> : public TrueType {
};

template <typename Arg>
struct IsPointer : public FalseType {
};

template <typename Arg>
struct IsPointer<Arg*> : public TrueType {
};

template <typename Arg>
struct AddConst {
	typedef Arg const Type;
};

template <typename Arg>
struct AddConst<Arg const> {
	typedef Arg const Type;
};

template <typename Arg>
struct RemoveConst {
	typedef Arg Type;
};

template <typename Arg>
struct RemoveConst<Arg const> {
	typedef Arg Type;
};

}} // namespaces

#endif // NEXTWEB_UTILS_TYPE_TRAITS_HPP_INCLUDED
