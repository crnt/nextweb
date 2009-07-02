#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "details/Range.hpp"
#include "details/StringUtils.hpp"

namespace nextweb { namespace tests {

class TrimTest : public CppUnit::TestFixture {

public:
	void testTrim();
	void testTrimLeft();
	void testTrimRight();

private:
	typedef std::vector<char> BytesType;
	typedef details::Range<char const*> RangeType;
	
	template <typename Cont> void testTrimWith();
	template <typename Cont> void testTrimLeftWith();
	template <typename Cont> void testTrimRightWith();

private:
	CPPUNIT_TEST_SUITE(TrimTest);
	CPPUNIT_TEST(testTrim);
	CPPUNIT_TEST(testTrimLeft);
	CPPUNIT_TEST(testTrimRight);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TrimTest, "TrimTest");
CPPUNIT_REGISTRY_ADD("TrimTest", "core");

void 
TrimTest::testTrim() {
	testTrimWith<BytesType>();
	testTrimWith<RangeType>();
	testTrimWith<std::string>();
}

void
TrimTest::testTrimLeft() {
	testTrimLeftWith<BytesType>();
	testTrimLeftWith<RangeType>();
	testTrimLeftWith<std::string>();
}

void
TrimTest::testTrimRight() {
	testTrimRightWith<BytesType>();
	testTrimRightWith<RangeType>();
	testTrimRightWith<std::string>();
}

template <typename Cont> void
TrimTest::testTrimWith() {
	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(std::string(), makeString(trim(std::string())));
	CPPUNIT_ASSERT_EQUAL(makeString(as<Cont>("test")), makeString(trim(as<Cont>("    test  "))));
}

template <typename Cont> void
TrimTest::testTrimLeftWith() {
	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(makeString(as<Cont>("test")), makeString(trimLeft(as<Cont>("   test"))));
}

template <typename Cont> void
TrimTest::testTrimRightWith() {
	using namespace nextweb::details;
	CPPUNIT_ASSERT_EQUAL(makeString(as<Cont>("test")), makeString(trimRight(as<Cont>("test  "))));
}

}} // namespaces
