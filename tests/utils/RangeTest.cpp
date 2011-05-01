#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <sstream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"

namespace nextweb { namespace tests {

class RangeTest : public CppUnit::TestFixture {

public:
	void testOutput();
	void testReversed();
	void testConstruct();
	void testComparision();
	void testReversedComparision();
	
private:
	template <typename Sequence> void testOutputWith();
	template <typename Sequence> void testReversedWith();
	template <typename Sequence> void testConstructWith();
	template <typename Sequence> void testComparisionWith();
	template <typename Sequence> void testReversedComparisionWith();
	
	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;

private:
	CPPUNIT_TEST_SUITE(RangeTest);
	CPPUNIT_TEST(testOutput);
	CPPUNIT_TEST(testReversed);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testComparision);
	CPPUNIT_TEST(testReversedComparision);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RangeTest, "RangeTest");
CPPUNIT_REGISTRY_ADD("RangeTest", "dependent");

void
RangeTest::testOutput() {
	testOutputWith<CharList>();
	testOutputWith<CharVector>();
	testOutputWith<std::string>();
	testOutputWith<CharSequence>();
}

void
RangeTest::testReversed() {
	testReversedWith<CharList>();
	testReversedWith<CharVector>();
	testReversedWith<std::string>();
	testReversedWith<CharSequence>();
}

void
RangeTest::testConstruct() {
	testConstructWith<CharList>();
	testConstructWith<CharVector>();
	testConstructWith<std::string>();
	testConstructWith<CharSequence>();
}

void
RangeTest::testComparision() {
	testComparisionWith<CharList>();
	testComparisionWith<CharVector>();
	testComparisionWith<std::string>();
	testComparisionWith<CharSequence>();
}

void
RangeTest::testReversedComparision() {
	testReversedComparisionWith<CharList>();
	testReversedComparisionWith<CharVector>();
	testReversedComparisionWith<std::string>();
	testReversedComparisionWith<CharSequence>();
}

template <typename Sequence> void
RangeTest::testOutputWith() {
	
	using namespace utils;
	std::stringstream stream;
	Sequence seq = as<Sequence>("test string");
	Range<typename Sequence::const_iterator> range(seq.begin(), seq.end());
	
	stream << range;
	CPPUNIT_ASSERT_EQUAL(makeString(seq), stream.str());
}

template <typename Sequence> void
RangeTest::testReversedWith() {

	using namespace utils;
	Sequence value = as<Sequence>("123 456");
	Range<typename Sequence::iterator> range(value.begin(), value.end());
	
	CPPUNIT_ASSERT_EQUAL(std::string("654 321"), std::string(range.rbegin(), range.rend()));
	CPPUNIT_ASSERT_EQUAL(std::string(value.rbegin(), value.rend()), std::string(range.rbegin(), range.rend()));
}

template <typename Sequence> void
RangeTest::testConstructWith() {

	using namespace utils;
	typedef Range<typename Sequence::const_iterator> TestRange;
	
	TestRange range;
	CPPUNIT_ASSERT_EQUAL(true, range.empty());
	CPPUNIT_ASSERT_EQUAL(static_cast<typename TestRange::size_type>(0), range.size());

	Sequence seq = as<Sequence>("12345");
	TestRange other = makeRange(seq);

	CPPUNIT_ASSERT_EQUAL(false, other.empty());	
	CPPUNIT_ASSERT_EQUAL(static_cast<typename TestRange::size_type>(5), other.size());
}

template <typename Sequence> void
RangeTest::testComparisionWith() {

	using namespace utils;
	Sequence seq = as<Sequence>("test string");
	Range<char const*> pattern = makeRange("test string");
	Range<typename Sequence::const_iterator> empty, range(seq.begin(), seq.end());

	CPPUNIT_ASSERT_EQUAL(true, range == pattern);
	CPPUNIT_ASSERT_EQUAL(true, range <= pattern);
	CPPUNIT_ASSERT_EQUAL(true, range >= pattern);
	CPPUNIT_ASSERT_EQUAL(false, range != pattern);

	CPPUNIT_ASSERT_EQUAL(true, empty < pattern);
	CPPUNIT_ASSERT_EQUAL(false, empty == pattern);
}

template <typename Sequence> void
RangeTest::testReversedComparisionWith() {

	using namespace utils;
	Sequence seq = as<Sequence>("test string");
	Range<char const*> source = makeRange("test string");
	Range<typename Sequence::const_reverse_iterator> range(seq.rbegin(), seq.rend());
	Range<Range<char const*>::const_reverse_iterator> empty, pattern(source.rbegin(), source.rend());
	
	CPPUNIT_ASSERT_EQUAL(true, range == pattern);
	CPPUNIT_ASSERT_EQUAL(true, range <= pattern);
	CPPUNIT_ASSERT_EQUAL(true, range >= pattern);
	CPPUNIT_ASSERT_EQUAL(false, range != pattern);

	CPPUNIT_ASSERT_EQUAL(true, empty < pattern);
	CPPUNIT_ASSERT_EQUAL(false, empty == pattern);
}

}} // namespaces
