/** @file AcceptTask.hpp */
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

#ifndef NEXTWEB_DETAILS_ACCEPT_TASK_HPP_INCLUDED
#define NEXTWEB_DETAILS_ACCEPT_TASK_HPP_INCLUDED

namespace nextweb { namespace details {

template <typename ThreadingModel>
class AcceptTask {
public:
	AcceptTask(ThreadingModel *object, typename ThreadingModel::SettingsType const &settings);
	void operator () () const;

private:
	ThreadingModel *object_;
	typename ThreadingModel::SettingsType const &settings_;
};

template <typename ThreadingModel> inline 
AcceptTask<ThreadingModel>::AcceptTask(ThreadingModel *object, typename ThreadingModel::SettingsType const &settings) :
	object_(object), settings_(settings)
{
}

template <typename ThreadingModel> inline void
AcceptTask<ThreadingModel>::operator () () const {
	object_->acceptLoop(settings_);
}

}} // namespaces

#endif // NEXTWEB_DETAILS_ACCEPT_TASK_HPP_INCLUDED
