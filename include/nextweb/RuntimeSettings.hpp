/** @file RuntimeSettings.hpp */
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

#ifndef NEXTWEB_RUNTIME_SETTINGS_HPP_INCLUDED
#define NEXTWEB_RUNTIME_SETTINGS_HPP_INCLUDED

#include <string>
#include <cstdio>
#include <cstddef>

#include "nextweb/Config.hpp"
#include "nextweb/Enumeration.hpp"

namespace nextweb {

struct NEXTWEB_API QueueParams {
	std::string name;
	std::size_t threadCount;
};

class NEXTWEB_API RuntimeSettings {

public:
	RuntimeSettings();
	virtual ~RuntimeSettings();

	enum ThreadingModel { SINGLE_THREADED, THREAD_PER_CONNECT, THREAD_POOL };
	
	virtual int getSocket() const = 0;
	virtual off_t getPostSizeThreshold() const;
	virtual ThreadingModel getThreadingModel() const = 0;
	
private:
	RuntimeSettings(RuntimeSettings const &);
	RuntimeSettings& operator = (RuntimeSettings const &);
};

class NEXTWEB_API ThreadPerConnectSettings : public RuntimeSettings {

public:
	ThreadPerConnectSettings();
	virtual ~ThreadPerConnectSettings();
	
	virtual std::size_t getThreadCount() const = 0;
	virtual ThreadingModel getThreadingModel() const;
	
private:
	ThreadPerConnectSettings(ThreadPerConnectSettings const &);
	ThreadPerConnectSettings& operator = (ThreadPerConnectSettings const &);
};

class NEXTWEB_API ThreadPoolSettings : public RuntimeSettings {

public:
	ThreadPoolSettings();
	virtual ~ThreadPoolSettings();
	
	virtual std::size_t getAcceptorCount() const = 0;
	virtual Enumeration<QueueParams>::PtrType getQueueParams() const = 0;
	virtual ThreadingModel getThreadingModel() const;

private:
	ThreadPoolSettings(ThreadPoolSettings const &);
	ThreadPoolSettings& operator = (ThreadPoolSettings const &);
};

} // namespace

#endif // NEXTWEB_RUNTIME_SETTINGS_HPP_INCLUDED
