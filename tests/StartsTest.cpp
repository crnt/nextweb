#include "acsetup.hpp"

#include <string>
#include <vector>
#include <cstring>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "details/Range.hpp"
#include "details/Functors.hpp"
#include "details/StringUtils.hpp"

namespace nextweb { namespace tests {

class StartsTest : public CppUnit::TestFixture {

public:
	void testEnds();
	void testEndsComparator();
	void testStarts();
	void testStartsComparator();
	void testEndsDifferent();
	void testStartsDifferent();

private:
	typedef std::vector<char> BytesType;
	typedef details::Range<char const*> RangeType;

	template <typename Cont> void testEndsWith();
	template <typename Cont> void testEndsComparatorWith();
	template <typename Cont> void testStartsWith();
	template <typename Cont> void testStartsComparatorWith();

private:
	CPPUNIT_TEST_SUITE(StartsTest);
	CPPUNIT_TEST(testEnds);
	CPPUNIT_TEST(testEndsComparator);
	CPPUNIT_TEST(testStarts);
	CPPUNIT_TEST(testStartsComparator);
	CPPUNIT_TEST(testEndsDifferent);
	CPPUNIT_TEST(testStartsDifferent);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StartsTest, "StartsTest");
CPPUNIT_REGISTRY_ADD("StartsTest", "core");

void
StartsTest::testEnds() {
	testEndsWith<BytesType>();
	testEndsWith<RangeType>();
	testEndsWith<std::string>();
}

void
StartsTest::testEndsComparator() {
	testEndsComparatorWith<BytesType>();
	testEndsComparatorWith<RangeType>();
	testEndsComparatorWith<std::string>();
}

void
StartsTest::testStarts() {
	testStartsWith<BytesType>();
	testStartsWith<RangeType>();
	testStartsWith<std::string>();
}

void
StartsTest::testStartsComparator() {
	testStartsComparatorWith<BytesType>();
	testStartsComparatorWith<RangeType>();
	testStartsComparatorWith<std::string>();
}
	
void
StartsTest::testEndsDifferent() {
	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<std::string>("testing"), as<BytesType>("ing")));
}

void
StartsTest::testStartsDifferent() {
	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<BytesType>("testing"), as<std::string>("test")));
}

template <typename Cont> inline void
StartsTest::testEndsWith() {

	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<Cont>("testing"), "ing"));
	CPPUNIT_ASSERT_EQUAL(true, endsWith("testing", as<Cont>("ing")));
	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<Cont>("testing"), as<Cont>("ing")));
}

template <typename Cont> inline void
StartsTest::testEndsComparatorWith() {

	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<Cont>("testiNg"), "ing", CIEqual<char>()));
	CPPUNIT_ASSERT_EQUAL(true, endsWith("testiNg", as<Cont>("ing"), CIEqual<char>()));
	CPPUNIT_ASSERT_EQUAL(true, endsWith(as<Cont>("testiNg"), as<Cont>("ing"), CIEqual<char>()));
}

template <typename Cont> inline void
StartsTest::testStartsWith() {

	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<Cont>("testing"), "test"));
	CPPUNIT_ASSERT_EQUAL(true, startsWith("testing", as<Cont>("test")));
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<Cont>("testing"), as<Cont>("test")));
}

template <typename Cont> inline void
StartsTest::testStartsComparatorWith() {

	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<Cont>("Testing"), "test", CIEqual<char>()));
	CPPUNIT_ASSERT_EQUAL(true, startsWith("Testing", as<Cont>("test"), CIEqual<char>()));
	CPPUNIT_ASSERT_EQUAL(true, startsWith(as<Cont>("Testing"), as<Cont>("test"), CIEqual<char>()));
}

}} // namespaces
