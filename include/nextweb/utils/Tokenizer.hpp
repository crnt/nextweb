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

#ifndef NEXTWEB_UTILS_TOKENIZER_HPP_INCLUDED
#define NEXTWEB_UTILS_TOKENIZER_HPP_INCLUDED

#include <iterator>
#include <functional>

#include "nextweb/Config.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Iterator.hpp"
#include "nextweb/utils/StringUtils.hpp"

namespace nextweb { namespace utils {

template <typename Pred>
class SplitIfMatch {

public:
	SplitIfMatch(Pred const &pred);
	template <typename Sequence> bool operator () (Sequence &head, Sequence &tail) const;

private:
	Pred pred_;
};

template <typename Char>
class SplitIfEqual {

public:
	SplitIfEqual(Char value);
	template <typename Sequence> bool operator () (Sequence &head, Sequence &tail) const;

private:
	Char value_;
};

template <typename Char>
class SplitFirstOf {

public:
	SplitFirstOf(Char const *charset);
	template <typename Sequence> bool operator () (Sequence &head, Sequence &tail) const;
	
private:
	Char const *charset_;
};

template <typename Sequence>
struct IteratorRange {
	typedef typename Sequence::const_iterator Iterator;
	typedef Range<Iterator> Type;
};

template <typename Sequence, typename Pred>
class TokenIterator : public Iterator<TokenIterator<Sequence, Pred>, std::forward_iterator_tag, Sequence const> {

public:
	TokenIterator(Pred const &pred);
	TokenIterator(typename IteratorRange<Sequence>::Type const &range, Pred const &pred);
	
	void increment();
	Sequence const& dereference() const;
	bool equal(TokenIterator<Sequence, Pred> const &other) const;

private:
	Pred pred_;
	mutable Sequence sequence_;
	typename IteratorRange<Sequence>::Type head_, tail_;
};

template <typename Sequence, typename Pred>
class Tokenizer {

public:
	Tokenizer(Sequence const &sequence, Pred const &pred);

	typedef TokenIterator<Sequence, Pred> iterator;
	typedef TokenIterator<Sequence, Pred> const_iterator;
	typedef typename std::iterator_traits<iterator>::value_type value_type;

	const_iterator end() const;
	const_iterator begin() const;

private:
	Pred pred_;
	Sequence sequence_;
};

template <typename Pred> NEXTWEB_INLINE 
SplitIfMatch<Pred>::SplitIfMatch(Pred const &pred) :
	pred_(pred)
{
}

template <typename Pred> template <typename Sequence> NEXTWEB_INLINE bool
SplitIfMatch<Pred>::operator () (Sequence &head, Sequence &tail) const {
	return splitIfOnce(tail, pred_, head, tail);
}

template <typename Char> NEXTWEB_INLINE 
SplitIfEqual<Char>::SplitIfEqual(Char value) :
	value_(value)
{
}

template <typename Char> template <typename Sequence> NEXTWEB_INLINE bool
SplitIfEqual<Char>::operator () (Sequence &head, Sequence &tail) const {
	return splitOnce(tail, value_, head, tail);
}

template <typename Char> NEXTWEB_INLINE
SplitFirstOf<Char>::SplitFirstOf(Char const *charset) :
	charset_(charset)
{
}

template <typename Char> template <typename Sequence> NEXTWEB_INLINE bool
SplitFirstOf<Char>::operator () (Sequence &head, Sequence &tail) const {
	return splitFirstOfOnce(tail, charset_, head, tail);
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE
TokenIterator<Sequence, Pred>::TokenIterator(Pred const &pred) :
	pred_(pred), sequence_(), head_(), tail_()
{
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE
TokenIterator<Sequence, Pred>::TokenIterator(typename IteratorRange<Sequence>::Type const &range, Pred const &pred) :
	pred_(pred), sequence_(), head_(), tail_(range)
{
	increment();
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE void
TokenIterator<Sequence, Pred>::increment() {
	pred_(head_, tail_);
	sequence_ = Sequence(head_.begin(), head_.end());
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE Sequence const&
TokenIterator<Sequence, Pred>::dereference() const {
	return sequence_;
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE bool
TokenIterator<Sequence, Pred>::equal(TokenIterator<Sequence, Pred> const &other) const {
	return (head_ == other.head_) && (tail_ == other.tail_);
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE 
Tokenizer<Sequence, Pred>::Tokenizer(Sequence const &sequence, Pred const &pred) :
	pred_(pred), sequence_(sequence)
{
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE typename Tokenizer<Sequence, Pred>::const_iterator
Tokenizer<Sequence, Pred>::end() const {
	return const_iterator(pred_);
}

template <typename Sequence, typename Pred> NEXTWEB_INLINE typename Tokenizer<Sequence, Pred>::const_iterator
Tokenizer<Sequence, Pred>::begin() const {
	typename IteratorRange<Sequence>::Type range(sequence_.begin(), sequence_.end());
	return const_iterator(range, pred_);
}

}} // namespaces

#endif // NEXTWEB_UTILS_TOKENIZER_HPP_INCLUDED
