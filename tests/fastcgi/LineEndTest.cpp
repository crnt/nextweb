

#include <list>
#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/fastcgi/impl/LineEndFilter.hpp"

namespace nextweb { namespace tests { 

class LineEndTest : public CppUnit::TestFixture {

public:
	void testEmpty();
	void testForward();
	void testReverse();

private:
	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	template <typename Sequence> void testEmptyWith();
	template <typename Sequence> void testForwardWith();
	template <typename Sequence> void testReverseWith();
	
private:
	CPPUNIT_TEST_SUITE(LineEndTest);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testForward);
	CPPUNIT_TEST(testReverse);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(LineEndTest);

void
LineEndTest::testEmpty() {
	testEmptyWith<CharList>();
	testEmptyWith<CharVector>();
	testEmptyWith<std::string>();
}

void
LineEndTest::testForward() {
	testForwardWith<CharList>();
	testForwardWith<CharVector>();
	testForwardWith<std::string>();
}

void
LineEndTest::testReverse() {
	testReverseWith<CharList>();
	testReverseWith<CharVector>();
	testReverseWith<std::string>();
}

template <typename Sequence> void
LineEndTest::testEmptyWith() {
	
	using namespace fastcgi;
	Sequence seq = as<Sequence>("");
	typedef LineEndFilter<typename Sequence::iterator> IteratorType;
	IteratorType begin(seq.begin(), seq.end()), end(seq.end(), seq.end());
	CPPUNIT_ASSERT_EQUAL(std::string(), std::string(begin, end));
}

template <typename Sequence> void
LineEndTest::testForwardWith() {
	using namespace fastcgi;
	Sequence seq = as<Sequence>("abcdef abcdef\r\n abcdef abcdef");
	CPPUNIT_ASSERT_EQUAL(std::string("abcdef abcdef abcdef abcdef"), makeString(makeLineEndFiltered(seq.begin(), seq.end())));
}

template <typename Sequence> void
LineEndTest::testReverseWith() {

	using namespace fastcgi;
	Sequence seq = as<Sequence>("abcdef abcdef\r\n abcdef abcdef");
	typedef LineEndFilter<typename Sequence::iterator> IteratorType;
	typedef std::reverse_iterator<IteratorType> ReverseIteratorType;
	
	utils::Range<IteratorType> range = makeLineEndFiltered(seq.begin(), seq.end());
	CPPUNIT_ASSERT_EQUAL(std::string("fedcba fedcba fedcba fedcba"), std::string(range.rbegin(), range.rend()));
}

}} // namespaces
