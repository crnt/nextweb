/** @file Mutex.hpp */
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

#ifndef NEXTWEB_DETAILS_MUTEX_HPP_INCLUDED
#define NEXTWEB_DETAILS_MUTEX_HPP_INCLUDED

#include <pthread.h>

#include "details/ScopedLock.hpp"

namespace nextweb { namespace details {

class Condition;

class MutexBase {

protected:
	MutexBase(int mode);
	virtual ~MutexBase();
	
	void lock();
	void unlock();
	bool trylock();
	
private:
	MutexBase(MutexBase const &);
	MutexBase& operator = (MutexBase const &);
	friend class Condition;

private:
	pthread_mutex_t impl_;
};

template <int Mode>
class ConcreteMutex : public MutexBase {

public:
	ConcreteMutex();
	virtual ~ConcreteMutex();

	typedef ConcreteMutex<Mode> MutexType;
	typedef ScopedLock<MutexType> Lock;

private:
	ConcreteMutex(ConcreteMutex const &);
	ConcreteMutex& operator = (ConcreteMutex const &);
	friend class ScopedLock<MutexType>;
};

typedef ConcreteMutex<PTHREAD_MUTEX_NORMAL> Mutex;
typedef ConcreteMutex<PTHREAD_MUTEX_RECURSIVE> RecursiveMutex;

template <int Mode> inline
ConcreteMutex<Mode>::ConcreteMutex() :
	MutexBase(Mode)
{
}

template <int Mode> inline
ConcreteMutex<Mode>::~ConcreteMutex() {
}

}} // namespaces

#endif // NEXTWEB_DETAILS_MUTEX_HPP_INCLUDED
