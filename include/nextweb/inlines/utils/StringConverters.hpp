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

#ifndef NEXTWEB_INLINES_UTILS_STRING_CONVERTERS_HPP_INCLUDED
#define NEXTWEB_INLINES_UTILS_STRING_CONVERTERS_HPP_INCLUDED

namespace nextweb { namespace utils {

NEXTWEB_INLINE
ConvertError::ConvertError() :
	Error("can not convert value")
{
}

NEXTWEB_INLINE void
ConvertError::throwUnless(bool condition) {
	if (!condition) throw ConvertError();
}

NEXTWEB_INLINE IntMax
IntConverter<true>::fromCharPtr(char const *value) {
	IntMax res;
	int success = sscanf(value, "%lld", &res);
	ConvertError::throwUnless(success > 0);
	return res;
}

NEXTWEB_INLINE UIntMax
IntConverter<false>::fromCharPtr(char const *value) {
	UIntMax res;
	int success = sscanf(value, "%llu", &res);
	ConvertError::throwUnless(success > 0);
	return res;
}

}} // namespaces

#endif // NEXTWEB_INLINES_UTILS_STRING_CONVERTERS_HPP_INCLUDED
