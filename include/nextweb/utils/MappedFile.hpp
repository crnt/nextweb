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

#ifndef NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED
#define NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED

#include <limits>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <algorithm>

#include "nextweb/Config.hpp"
#include "nextweb/Shared.hpp"
#include "nextweb/SharedPtr.hpp"
#include "nextweb/utils/Integer.hpp"
#include "nextweb/utils/Iterator.hpp"
#include "nextweb/utils/Resource.hpp"

namespace nextweb { namespace utils {

struct DescriptorTraits {
	void destroy(int fdes);
	static int defaultValue();
};

class MappedFileImpl : public Shared {

public:
	typedef void* Pointer;
	typedef std::size_t SizeType;

	MappedFileImpl(char const *name, bool readOnly, SizeType pageSize);
	virtual ~MappedFileImpl();
	
	SizeType size() const;
	SizeType pageSize() const;
	void resize(SizeType newsize);
	
	void reservePage(SizeType position);
	void releasePage(SizeType position) throw ();

private:
	MappedFileImpl(MappedFileImpl const &);
	MappedFileImpl& operator = (MappedFileImpl const &);
	void mapFile(SizeType size);
	
	static Pointer const INVALID_POINTER;
	
private:
	SizeType size_;
	Pointer pointer_;
	bool const readOnly_;
	Resource<int, DescriptorTraits> fdes_;
};

template <typename Char, typename Impl>
class MappedFileIteratorBase {

public:
	typedef typename Impl::SizeType SizeType;
	typedef typename MakeSigned<typename Impl::SizeType>::Type DistanceType;
	
	void increment();
	void decrement();
	void advance(DistanceType dist);
	
	bool equal(MappedFileIteratorBase<Char, Impl> const &other) const;
	DistanceType distance(MappedFileIteratorBase<Char, Impl> const &other) const;
	void swap(MappedFileIteratorBase<Char, Impl> &other) throw ();
	
protected:
	MappedFileIteratorBase();
	virtual ~MappedFileIteratorBase();
	
	MappedFileIteratorBase(SizeType pos, SharedPtr<Impl> const &impl);
	MappedFileIteratorBase(MappedFileIteratorBase<Char, Impl> const &other);
	
	SizeType position() const;
	SharedPtr<Impl>& implementation();
	SharedPtr<Impl> const& implementation() const;
	
private:
	void reservePage();
	void releasePage() throw ();

private:
	SizeType pos_;
	SharedPtr<Impl> impl_;
};

template <typename Char, typename Impl>
class ReadableMappedFileIterator : 
	public MappedFileIteratorBase<Char, Impl>,
	public Iterator<ReadableMappedFileIterator<Char, Impl>, std::random_access_iterator_tag, Char>
{
public:
	typedef ReadableMappedFileIterator<Char, Impl> Type;
	typedef typename MappedFileIteratorBase<Char, Impl>::SizeType SizeType;
	
	ReadableMappedFileIterator();
	ReadableMappedFileIterator(SizeType pos, SharedPtr<Impl> const &impl);
	ReadableMappedFileIterator(ReadableMappedFileIterator<Char, Impl> const &other);
	ReadableMappedFileIterator<Char, Impl>& operator = (ReadableMappedFileIterator<Char, Impl> const &other);
	
	Char const& dereference() const;
};

template <typename Char, typename Impl>
class WriteableMappedFileIterator : 
	public MappedFileIteratorBase<Char, Impl>,
	public Iterator<WriteableMappedFileIterator<Char, Impl>, std::random_access_iterator_tag, Char>
{
public:
	typedef WriteableMappedFileIterator<Char, Impl> Type;
	typedef typename MappedFileIteratorBase<Char, Impl>::SizeType SizeType;
	
	WriteableMappedFileIterator();
	WriteableMappedFileIterator(SizeType pos, SharedPtr<Impl> const &impl);
	WriteableMappedFileIterator(WriteableMappedFileIterator<Char, Impl> const &other);
	WriteableMappedFileIterator<Char, Impl>& operator = (WriteableMappedFileIterator<Char, Impl> const &other);
	
	Char& dereference();
	operator ReadableMappedFileIterator<Char, Impl> () const;
};

template <typename Char, typename Impl>
class ReadableMappedFileReferenceProxy {

public:
	typedef typename Impl::SizeType SizeType;
	typedef ReadableMappedFileReferenceProxy<Char, Impl> Type;

	ReadableMappedFileReferenceProxy(SizeType pos, SharedPtr<Impl> const &impl);
	ReadableMappedFileReferenceProxy(ReadableMappedFileReferenceProxy<Char, Impl> const &other);
	ReadableMappedFileReferenceProxy<Char, Impl>& operator = (ReadableMappedFileReferenceProxy<Char, Impl> const &other);
	operator Char const& () const;

private:
	ReadableMappedFileIterator<Char, Impl> iter_;
};

template <typename Char, typename Impl>
class WriteableMappedFileReferenceProxy {

public:
	typedef typename Impl::SizeType SizeType;
	typedef WriteableMappedFileReferenceProxy<Char, Impl> Type;

	WriteableMappedFileReferenceProxy(SizeType pos, SharedPtr<Impl> const &impl);
	WriteableMappedFileReferenceProxy(WriteableMappedFileReferenceProxy<Char, Impl> const &other);
	WriteableMappedFileReferenceProxy<Char, Impl>& operator = (WriteableMappedFileReferenceProxy<Char, Impl> const &other);
	
	operator Char& ();
	operator Char const& () const;

private:
	WriteableMappedFileIterator<Char, Impl> iter_;
};

template <typename Char, typename Impl> NEXTWEB_INLINE
MappedFileIteratorBase<Char, Impl>::MappedFileIteratorBase() :
	pos_(std::numeric_limits<SizeType>::max()), impl_()
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE
MappedFileIteratorBase<Char, Impl>::~MappedFileIteratorBase() {
	releasePage();
}

template <typename Char, typename Impl> NEXTWEB_INLINE
MappedFileIteratorBase<Char, Impl>::MappedFileIteratorBase(typename MappedFileIteratorBase<Char, Impl>::SizeType pos, SharedPtr<Impl> const &impl) :
	pos_(pos), impl_(impl)
{
	reservePage();
}

template <typename Char, typename Impl> NEXTWEB_INLINE
MappedFileIteratorBase<Char, Impl>::MappedFileIteratorBase(MappedFileIteratorBase<Char, Impl> const &other) :
	pos_(other.pos_), impl_(other.impl_)
{
	reservePage();
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::swap(MappedFileIteratorBase<Char, Impl> &other) throw () {
	swap(impl_, other.impl_);
	std::swap(pos_, other.pos_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::increment() {
	assert(impl_);
	releasePage();
	pos_ += sizeof(Char);
	reservePage();
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::decrement() {
	assert(impl_);
	releasePage();
	pos_ -= sizeof(Char);
	reservePage();
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::advance(typename MappedFileIteratorBase<Char, Impl>::DistanceType dist) {
	assert(impl_);
	releasePage();
	pos_ += dist * sizeof(Char);
	reservePage();
}

template <typename Char, typename Impl> NEXTWEB_INLINE bool
MappedFileIteratorBase<Char, Impl>::equal(MappedFileIteratorBase<Char, Impl> const &other) const {
	assert(impl_ && (impl_ == other.impl_));
	return pos_ == other.pos_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename MappedFileIteratorBase<Char, Impl>::DistanceType
MappedFileIteratorBase<Char, Impl>::distance(MappedFileIteratorBase<Char, Impl> const &other) const {
	assert(impl_ && (impl_ == other.impl_));
	return static_cast<DistanceType>(other.pos_ - pos_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename MappedFileIteratorBase<Char, Impl>::SizeType
MappedFileIteratorBase<Char, Impl>::position() const {
	return pos_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE SharedPtr<Impl>&
MappedFileIteratorBase<Char, Impl>::implementation() {
	return impl_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE SharedPtr<Impl> const&
MappedFileIteratorBase<Char, Impl>::implementation() const {
	return impl_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::reservePage() {
	impl_->reservePage(pos_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::releasePage() throw () {
	impl_->releasePage(pos_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
ReadableMappedFileIterator<Char, Impl>::ReadableMappedFileIterator() :
	MappedFileIteratorBase<Char, Impl>()
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
ReadableMappedFileIterator<Char, Impl>::ReadableMappedFileIterator(typename ReadableMappedFileIterator<Char, Impl>::SizeType pos, SharedPtr<Impl> const &impl) :
	MappedFileIteratorBase<Char, Impl>(pos, impl)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
ReadableMappedFileIterator<Char, Impl>::ReadableMappedFileIterator(ReadableMappedFileIterator<Char, Impl> const &other) :
	MappedFileIteratorBase<Char, Impl>(other)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE ReadableMappedFileIterator<Char, Impl>&
ReadableMappedFileIterator<Char, Impl>::operator = (ReadableMappedFileIterator<Char, Impl> const &other) {
	ReadableMappedFileIterator<Char, Impl> copy(other);
	swap(copy);
	return *this;
}

template <typename Char, typename Impl> NEXTWEB_INLINE Char const&
ReadableMappedFileIterator<Char, Impl>::dereference() const {
	SizeType pos = this->position();
	SharedPtr<Impl> const& impl = this->implementation();
	typename Impl::Pointer ptr = impl->at(pos * sizeof(Char));
	return *(static_cast<Char const*>(ptr));
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
WriteableMappedFileIterator<Char, Impl>::WriteableMappedFileIterator() :
	MappedFileIteratorBase<Char, Impl>()
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
WriteableMappedFileIterator<Char, Impl>::WriteableMappedFileIterator(typename WriteableMappedFileIterator<Char, Impl>::SizeType pos, SharedPtr<Impl> const &impl) :
	MappedFileIteratorBase<Char, Impl>(pos, impl)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
WriteableMappedFileIterator<Char, Impl>::WriteableMappedFileIterator(WriteableMappedFileIterator<Char, Impl> const &other) :
	MappedFileIteratorBase<Char, Impl>(other)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE WriteableMappedFileIterator<Char, Impl>&
WriteableMappedFileIterator<Char, Impl>::operator = (WriteableMappedFileIterator<Char, Impl> const &other) {
	WriteableMappedFileIterator<Char, Impl> copy(other);
	swap(copy);
	return *this;
}

template <typename Char, typename Impl> NEXTWEB_INLINE Char&
WriteableMappedFileIterator<Char, Impl>::dereference() {
	SizeType pos = this->position();
	SharedPtr<Impl>& impl = this->implementation();
	typename Impl::Pointer ptr = impl->at(pos * sizeof(Char));
	return *(static_cast<Char*>(ptr));
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFileIterator<Char, Impl>::operator ReadableMappedFileIterator<Char, Impl> () const {
	return ReadableMappedFileIterator<Char, Impl>(this->position(), this->impl());
}

template <typename Char, typename Impl> NEXTWEB_INLINE
ReadableMappedFileReferenceProxy<Char, Impl>::ReadableMappedFileReferenceProxy(typename ReadableMappedFileReferenceProxy<Char, Impl>::SizeType pos, SharedPtr<Impl> const &impl) :
	iter_(ReadableMappedFileIterator<Char, Impl>(pos, impl))
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE
ReadableMappedFileReferenceProxy<Char, Impl>::ReadableMappedFileReferenceProxy(ReadableMappedFileReferenceProxy<Char, Impl> const &other) :
	iter_(other.iter_)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE ReadableMappedFileReferenceProxy<Char, Impl>&
ReadableMappedFileReferenceProxy<Char, Impl>::operator = (ReadableMappedFileReferenceProxy<Char, Impl> const &other) {
	iter_ = other.iter_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE
ReadableMappedFileReferenceProxy<Char, Impl>::operator Char const& () const {
	return iter_.dereference();
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFileReferenceProxy<Char, Impl>::WriteableMappedFileReferenceProxy(SizeType pos, SharedPtr<Impl> const &impl) :
	iter_(WriteableMappedFileIterator<Char, Impl>(pos, impl))
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFileReferenceProxy<Char, Impl>::WriteableMappedFileReferenceProxy(WriteableMappedFileReferenceProxy<Char, Impl> const &other) :
	iter_(other.iter_)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE WriteableMappedFileReferenceProxy<Char, Impl>&
WriteableMappedFileReferenceProxy<Char, Impl>::operator = (WriteableMappedFileReferenceProxy<Char, Impl> const &other) {
	iter_ = other.iter_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFileReferenceProxy<Char, Impl>::operator Char& () {
	return iter_.dereference();
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFileReferenceProxy<Char, Impl>::operator Char const& () const {
	return static_cast<typename ReadableMappedFileIterator<Char, Impl>::Type>(iter_).dereference();
}

}} // namespaces

#endif // NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED
