// Copyright (C) 2009 Oleg Obolenskiy <highpower@yandex-team.ru>

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

	Pointer at(SizeType position);
	Pointer at(SizeType position) const;

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

	static SizeType const INVALID_POSITION;

private:
	SizeType pos_;
	SharedPtr<Impl> impl_;
};

template <typename Char, typename Impl>
class ReadableMappedFileIterator : 
	public MappedFileIteratorBase<Char, Impl>,
	public Iterator<ReadableMappedFileIterator<Char, Impl>, std::random_access_iterator_tag, Char const>
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

template <typename Char, typename Impl>
class ReadableMappedFile {

public:
	typedef Char const* pointer;
	typedef Char const value_type;
	
	typedef typename Impl::SizeType size_type;
	typedef typename MakeSigned<typename Impl::SizeType>::Type difference_type;

	typedef ReadableMappedFileReferenceProxy<Char, Impl> reference;
	typedef ReadableMappedFileReferenceProxy<Char, Impl> const_reference;

	
	typedef ReadableMappedFileIterator<Char, Impl> iterator;
	typedef ReadableMappedFileIterator<Char, Impl> const_iterator;
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	ReadableMappedFile();
	ReadableMappedFile(char const *name, size_type pageSize);
	ReadableMappedFile(ReadableMappedFile<Char, Impl> const &other);
	ReadableMappedFile<Char, Impl>& operator = (ReadableMappedFile<Char, Impl> const &other);
	
	size_type size() const;
	const_iterator end() const;
	const_iterator begin() const;
	const_reverse_iterator rend() const;
	const_reverse_iterator rbegin() const;
	const_reference at(size_type position) const;

	void open(char const *name, size_type pageSize);
	void swap(ReadableMappedFile<Char, Impl> &other) throw ();

protected:
	SharedPtr<Impl>& implementation();
	SharedPtr<Impl> const& implementation() const;

private:
	SharedPtr<Impl> impl_;
};

template <typename Char, typename Impl>
class WriteableMappedFile : public ReadableMappedFile<Char, Impl> {

public:
	typedef Char* pointer;
	typedef Char value_type;
	
	typedef typename Impl::SizeType size_type;
	typedef WriteableMappedFileReferenceProxy<Char, Impl> reference;
	
	typedef WriteableMappedFileIterator<Char, Impl> iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	
	WriteableMappedFile();
	WriteableMappedFile(char const *name, size_type pageSize);
	WriteableMappedFile(WriteableMappedFile<Char, Impl> const &other);
	WriteableMappedFile<Char, Impl>& operator = (WriteableMappedFile<Char, Impl> const &other);
	
	iterator end();
	iterator begin();
	reverse_iterator rend();
	reverse_iterator rbegin();

	void resize(size_type newSize);
	reference at(size_type position);
	void open(char const *name, size_type pageSize);
};

template <typename Char, typename Impl> typename MappedFileIteratorBase<Char, Impl>::SizeType const
MappedFileIteratorBase<Char, Impl>::INVALID_POSITION = std::numeric_limits<SizeType>::max();

template <typename Char, typename Impl> NEXTWEB_INLINE
MappedFileIteratorBase<Char, Impl>::MappedFileIteratorBase() :
	pos_(INVALID_POSITION), impl_()
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
	if (INVALID_POSITION != pos_) {
		impl_->reservePage(pos_);
	}
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
MappedFileIteratorBase<Char, Impl>::releasePage() throw () {
	if (INVALID_POSITION != pos_) {
		impl_->releasePage(pos_);
	}
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

template <typename Char, typename Impl> NEXTWEB_INLINE
ReadableMappedFile<Char, Impl>::ReadableMappedFile() :
	impl_()
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE
ReadableMappedFile<Char, Impl>::ReadableMappedFile(char const *name, typename ReadableMappedFile<Char, Impl>::size_type pageSize) :
	impl_()
{
	open(name, pageSize);
}

template <typename Char, typename Impl> NEXTWEB_INLINE 
ReadableMappedFile<Char, Impl>::ReadableMappedFile(ReadableMappedFile<Char, Impl> const &other) :
	impl_(other.impl_)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE ReadableMappedFile<Char, Impl>& 
ReadableMappedFile<Char, Impl>::operator = (ReadableMappedFile<Char, Impl> const &other) {
	ReadableMappedFile<Char, Impl> copy(other);
	swap(copy);
	return *this;
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename ReadableMappedFile<Char, Impl>::size_type 
ReadableMappedFile<Char, Impl>::size() const {
	return impl_->size();
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename ReadableMappedFile<Char, Impl>::const_iterator 
ReadableMappedFile<Char, Impl>::end() const {
	return const_iterator(size(), impl_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename ReadableMappedFile<Char, Impl>::const_iterator
ReadableMappedFile<Char, Impl>::begin() const {
	return const_iterator(0, impl_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename ReadableMappedFile<Char, Impl>::const_reverse_iterator
ReadableMappedFile<Char, Impl>::rend() const {
	return const_reverse_iterator(begin());
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename ReadableMappedFile<Char, Impl>::const_reverse_iterator
ReadableMappedFile<Char, Impl>::rbegin() const {
	return const_reverse_iterator(end());
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename ReadableMappedFile<Char, Impl>::const_reference
ReadableMappedFile<Char, Impl>::at(ReadableMappedFile<Char, Impl>::size_type position) const {
	return const_reference(position, impl_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
ReadableMappedFile<Char, Impl>::open(char const *name, ReadableMappedFile<Char, Impl>::size_type pageSize) {
	SharedPtr<Impl> &impl = implementation();
	impl.reset(new Impl(name, true, pageSize));
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
ReadableMappedFile<Char, Impl>::swap(ReadableMappedFile<Char, Impl> &other) throw () {
	impl_.swap(other.impl_);
}

template <typename Char, typename Impl> NEXTWEB_INLINE SharedPtr<Impl>&
ReadableMappedFile<Char, Impl>::implementation() {
	return impl_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE SharedPtr<Impl> const&
ReadableMappedFile<Char, Impl>::implementation() const {
	return impl_;
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFile<Char, Impl>::WriteableMappedFile() :
	ReadableMappedFile<Char, Impl>()
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFile<Char, Impl>::WriteableMappedFile(char const *name, typename WriteableMappedFile<Char, Impl>::size_type pageSize) :
	ReadableMappedFile<Char, Impl>()
{
	open(name, pageSize);
}

template <typename Char, typename Impl> NEXTWEB_INLINE
WriteableMappedFile<Char, Impl>::WriteableMappedFile(WriteableMappedFile<Char, Impl> const &other) :
	ReadableMappedFile<Char, Impl>(other)
{
}

template <typename Char, typename Impl> NEXTWEB_INLINE WriteableMappedFile<Char, Impl>&
WriteableMappedFile<Char, Impl>::operator = (WriteableMappedFile<Char, Impl> const &other) {
	WriteableMappedFile<Char, Impl> copy(other);
	swap(copy);
	return *this;
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename WriteableMappedFile<Char, Impl>::iterator 
WriteableMappedFile<Char, Impl>::end() {
	SharedPtr<Impl> &impl = this->implementation();
	return iterator(impl->size(), impl);
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename WriteableMappedFile<Char, Impl>::iterator
WriteableMappedFile<Char, Impl>::begin() {
	SharedPtr<Impl> &impl = this->implementation();
	return iterator(0, impl);
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename WriteableMappedFile<Char, Impl>::reverse_iterator
WriteableMappedFile<Char, Impl>::rend() {
	return reverse_iterator(begin());
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename WriteableMappedFile<Char, Impl>::reverse_iterator
WriteableMappedFile<Char, Impl>::rbegin() {
	return reverse_iterator(end());
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
WriteableMappedFile<Char, Impl>::resize(typename WriteableMappedFile<Char, Impl>::size_type newSize) {
	SharedPtr<Impl> &impl = this->implementation();
	impl->resize(newSize);
}

template <typename Char, typename Impl> NEXTWEB_INLINE typename WriteableMappedFile<Char, Impl>::reference
WriteableMappedFile<Char, Impl>::at(typename WriteableMappedFile<Char, Impl>::size_type position) {
	SharedPtr<Impl> &impl = this->implementation();
	return reference(position, impl);
}

template <typename Char, typename Impl> NEXTWEB_INLINE void
WriteableMappedFile<Char, Impl>::open(char const *name, typename WriteableMappedFile<Char, Impl>::size_type pageSize) {
	SharedPtr<Impl> &impl = this->implementation();
	impl.reset(new Impl(name, false, pageSize));
}

}} // namespaces

#ifndef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/MappedFile.hpp"
#endif 

#endif // NEXTWEB_UTILS_MAPPED_FILE_HPP_INCLUDED
