#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Functors.hpp"
#include "nextweb/utils/StringUtils.hpp"

namespace nextweb { namespace tests {

class StringTest : public CppUnit::TestFixture {

public:
	void testTrim();
	void testTrimLeft();
	void testTrimRight();

	void testSplit();
	void testSplitIf();
	void testSplitFirstOf();
	void testSplitReversed();
	void testStartsEnds();

private:
	template <typename Sequence> void testTrimWith();
	template <typename Sequence> void testTrimLeftWith();
	template <typename Sequence> void testTrimRightWith();
	template <typename Sequence> void testSplitWith();
	template <typename Sequence> void testSplitIfWith();
	template <typename Sequence> void testSplitFirstOfWith();
	template <typename Sequence> void testSplitReversedWith();
	template <typename Sequence> void testStartsEndsWith();

	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;

private:
	CPPUNIT_TEST_SUITE(StringTest);
	CPPUNIT_TEST(testTrim);
	CPPUNIT_TEST(testTrimLeft);
	CPPUNIT_TEST(testTrimRight);
	CPPUNIT_TEST(testSplit);
	CPPUNIT_TEST(testSplitIf);
	CPPUNIT_TEST(testSplitFirstOf);
	CPPUNIT_TEST(testSplitReversed);
	CPPUNIT_TEST(testStartsEnds);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringTest, "StringTest");
CPPUNIT_REGISTRY_ADD("StringTest", "dependent");

void
StringTest::testTrim() {
	testTrimWith<CharList>();
	testTrimWith<CharVector>();
	testTrimWith<std::string>();
	testTrimWith<CharSequence>();
}

void
StringTest::testTrimLeft() {
	testTrimLeftWith<CharList>();
	testTrimLeftWith<CharVector>();
	testTrimLeftWith<std::string>();
	testTrimLeftWith<CharSequence>();
}

void
StringTest::testTrimRight() {
	testTrimRightWith<CharList>();
	testTrimRightWith<CharVector>();
	testTrimRightWith<std::string>();
	testTrimRightWith<CharSequence>();
}

void
StringTest::testSplit() {
	testSplitWith<CharList>();
	testSplitWith<CharVector>();
	testSplitWith<std::string>();
	testSplitWith<CharSequence>();
}

void
StringTest::testSplitIf() {
	testSplitIfWith<CharList>();
	testSplitIfWith<CharVector>();
	testSplitIfWith<std::string>();
	testSplitIfWith<CharSequence>();
}

void
StringTest::testSplitFirstOf() {
	testSplitFirstOfWith<CharList>();
	testSplitFirstOfWith<CharVector>();
	testSplitFirstOfWith<std::string>();
	testSplitFirstOfWith<CharSequence>();
}

void
StringTest::testSplitReversed() {
	testSplitReversedWith<CharList>();
	testSplitReversedWith<CharVector>();
	testSplitReversedWith<std::string>();
	testSplitReversedWith<CharSequence>();
}

void
StringTest::testStartsEnds() {
	testStartsEndsWith<CharList>();
	testStartsEndsWith<CharVector>();
	testStartsEndsWith<std::string>();
	testStartsEndsWith<CharSequence>();
}

template <typename Sequence> inline void
StringTest::testTrimWith() {

	using namespace utils;
	Sequence value = as<Sequence>(""), result = trim(value);
	CPPUNIT_ASSERT_EQUAL(std::string(), makeString(result));
	CPPUNIT_ASSERT_EQUAL(std::string("test"), makeString(trim(as<Sequence>("  test  "))));
}

template <typename Sequence> inline void
StringTest::testTrimLeftWith() {

	using namespace utils;
	CPPUNIT_ASSERT_EQUAL(std::string("test  "), makeString(trimLeft(as<Sequence>("   test  "))));
}

template <typename Sequence> inline void
StringTest::testTrimRightWith() {

	using namespace utils;
	CPPUNIT_ASSERT_EQUAL(std::string("  test"), makeString(trimRight(as<Sequence>("  test  "))));
}

template <typename Sequence> inline void
StringTest::testSplitWith() {

	using namespace utils;
	
	Sequence head, tail = as<Sequence>("one two");
	CPPUNIT_ASSERT_EQUAL(true, (splitOnce(tail, ' ', head, tail)));
	CPPUNIT_ASSERT_EQUAL(false, (splitOnce(tail, ' ', head, tail)));
	
	tail = as<Sequence>("best test best test best");
	while (!tail.empty()) {
		splitOnce(tail, ' ', head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename Sequence::size_type>(4), head.size());
	}
	tail = as<Sequence>("best test best test best");
	while (!tail.empty()) {
		splitOnce(tail, "test", head, tail);
		CPPUNIT_ASSERT_EQUAL(std::string("best"), makeString(trim(head)));
		tail = trim(tail);
	}
	tail = as<Sequence>("best test best test best");
	while (!tail.empty()) {
		splitOnce(tail, makeRange("test"), head, tail);
		CPPUNIT_ASSERT_EQUAL(std::string("best"), makeString(trim(head)));
		tail = trim(tail);
	}
}

template <typename Sequence> inline void
StringTest::testSplitIfWith() {

	using namespace utils;
	Sequence head, tail = as<Sequence>("test test test test");
	while (!tail.empty()) {
		splitIfOnce(tail, IsSpace<char>(), head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename Sequence::size_type>(4), head.size());
		CPPUNIT_ASSERT_EQUAL(std::string("test"), makeString(head));
	}
}

template <typename Sequence> inline void
StringTest::testSplitFirstOfWith() {
	
	using namespace utils;
	Sequence head, tail = as<Sequence>("a&b;c&d;e&f");
	while (!tail.empty()) {
		splitFirstOfOnce(tail, "&;", head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename Sequence::size_type>(1), head.size());
	}
	tail = as<Sequence>("a&b;c&d;e&f");
	while (!tail.empty()) {
		splitFirstOfOnce(tail, makeRange("&;"), head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename Sequence::size_type>(1), head.size());
	}
}

template <typename Sequence> inline void
StringTest::testSplitReversedWith() {

	using namespace utils;
	typedef Range<typename Sequence::const_reverse_iterator> TestRange;
	Sequence seq = as<Sequence>("test test test test");
	TestRange head, tail(seq.rbegin(), seq.rend());
	
	while (!tail.empty()) {
		splitIfOnce(tail, IsSpace<char>(), head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename TestRange::size_type>(4), head.size());
		CPPUNIT_ASSERT_EQUAL(std::string("tset"), makeString(head));
	}
}

template <typename Sequence> inline void
StringTest::testStartsEndsWith() {
	
	using namespace utils;
	
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<Sequence>("testing"), "test"));
	CPPUNIT_ASSERT_EQUAL(false, startsWith(as<Sequence>("production"), "test"));
	CPPUNIT_ASSERT_EQUAL(true, startsWith("testing", as<Sequence>("test")));
	CPPUNIT_ASSERT_EQUAL(false, startsWith("production", as<Sequence>("test")));
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<Sequence>("testing"), as<Sequence>("test")));
	CPPUNIT_ASSERT_EQUAL(false, startsWith(as<Sequence>("production"), as<Sequence>("test")));

	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<Sequence>("testing"), "ing"));
	CPPUNIT_ASSERT_EQUAL(false, endsWith(as<Sequence>("production"), "ing"));
	CPPUNIT_ASSERT_EQUAL(true, endsWith("testing", as<Sequence>("ing")));
	CPPUNIT_ASSERT_EQUAL(false, endsWith("production", as<Sequence>("ing")));
	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<Sequence>("testing"), as<Sequence>("ing")));
	CPPUNIT_ASSERT_EQUAL(false, endsWith(as<Sequence>("production"), as<Sequence>("ing")));
}

}} // namespaces
