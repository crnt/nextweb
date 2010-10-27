#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Tokenizer.hpp"

namespace nextweb { namespace tests {

class TokenizerTest : public CppUnit::TestFixture {

public:
	void testTokenizerIf();
	void testTokenizerEqual();
	void testTokenizerFirstOf();
	void testTokenizerReversed();

private:
	template <typename Sequence> void testTokenizerIfWith();
	template <typename Sequence> void testTokenizerEqualWith();
	template <typename Sequence> void testTokenizerFirstOfWith();
	template <typename Sequence> void testTokenizerReversedWith();

	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;

private:
	CPPUNIT_TEST_SUITE(TokenizerTest);
	CPPUNIT_TEST(testTokenizerIf);
	CPPUNIT_TEST(testTokenizerEqual);
	CPPUNIT_TEST(testTokenizerFirstOf);
	CPPUNIT_TEST(testTokenizerReversed);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TokenizerTest, "TokenizerTest");
CPPUNIT_REGISTRY_ADD("TokenizerTest", "dependent");

void
TokenizerTest::testTokenizerIf() {
	testTokenizerIfWith<CharList>();
	testTokenizerIfWith<CharVector>();
	testTokenizerIfWith<std::string>();
	testTokenizerIfWith<CharSequence>();
}

void
TokenizerTest::testTokenizerEqual() {
	testTokenizerEqualWith<CharList>();
	testTokenizerEqualWith<CharVector>();
	testTokenizerEqualWith<std::string>();
	testTokenizerEqualWith<CharSequence>();
}

void
TokenizerTest::testTokenizerFirstOf() {
	testTokenizerFirstOfWith<CharList>();
	testTokenizerFirstOfWith<CharVector>();
	testTokenizerFirstOfWith<std::string>();
	testTokenizerFirstOfWith<CharSequence>();
}

void
TokenizerTest::testTokenizerReversed() {
	testTokenizerReversedWith<CharList>();
	testTokenizerReversedWith<CharVector>();
	testTokenizerReversedWith<std::string>();
	testTokenizerReversedWith<CharSequence>();
}

template <typename Sequence> void
TokenizerTest::testTokenizerIfWith() {

	using namespace utils;

	typedef IsSpace<char> SpaceChecker;
	typedef SplitIfMatch<SpaceChecker> Matcher;
	typedef Tokenizer<Sequence, Matcher> TestTokenizer;
	char const* values[] = { "123", "456", "789", "012", "345" };
	
	TestTokenizer tok(as<Sequence>("123 456 789 012 345"), Matcher(SpaceChecker()));
	typename TestTokenizer::const_iterator i = tok.begin(), end = tok.end();
	
	std::size_t count = 0;
	for (; i != end; ++i, ++count) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[count]), makeString(*i));
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), count);
}

template <typename Sequence> void
TokenizerTest::testTokenizerEqualWith() {

	using namespace utils;

	typedef SplitIfEqual<char> Matcher;
	typedef Tokenizer<Sequence, Matcher> TestTokenizer;
	char const* values[] = { "123", "456", "789", "012", "345" };
	
	TestTokenizer tok(as<Sequence>("123 456 789 012 345"), Matcher(' '));
	typename TestTokenizer::const_iterator i = tok.begin(), end = tok.end();
	
	std::size_t count = 0;
	for (; i != end; ++i, ++count) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[count]), makeString(*i));
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), count);
}

template <typename Sequence> void
TokenizerTest::testTokenizerFirstOfWith() {

	using namespace utils;

	typedef SplitFirstOf<char> Matcher;
	typedef Tokenizer<Sequence, Matcher> TestTokenizer;
	char const* values[] = { "123", "456", "789", "012", "345" };
	
	TestTokenizer tok(as<Sequence>("123 456 789 012 345"), Matcher(" \t"));
	typename TestTokenizer::const_iterator i = tok.begin(), end = tok.end();
	
	std::size_t count = 0;
	for (; i != end; ++i, ++count) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[count]), makeString(*i));
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), count);
}

template <typename Sequence> void
TokenizerTest::testTokenizerReversedWith() {

	using namespace utils;
	
	typedef SplitIfEqual<char> Matcher;
	typedef Range<typename Sequence::const_reverse_iterator> TestRange;
	typedef Tokenizer<TestRange, Matcher> TestTokenizer;
	
	Sequence seq = as<Sequence>("test test test test test");
	TestTokenizer tok(TestRange(seq.rbegin(), seq.rend()), Matcher(' '));
	for (typename TestTokenizer::const_iterator i = tok.begin(), end = tok.end(); i != end; ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string("tset"), makeString(*i));
	}
}

}} // namespaces
