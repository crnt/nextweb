#include "acsetup.hpp"

#include <list>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Integer.hpp"
#include "nextweb/utils/TypeList.hpp"
#include "nextweb/utils/StringConverters.hpp"

namespace nextweb { namespace tests {

class ConvertersTest : public CppUnit::TestFixture {

public:
	void testString();
	void testRanges();
	void testIntegers();
	void testBadValue();
	void testOverflow();
	void testEmptyToInteger();

private:
	template <typename List> void testRangesWith();
	template <typename List> void testIntegersWith();

	template <typename X> void testRangeConversionWith();
	template <typename X> void testIntegerConversionWith();

private:	
	CPPUNIT_TEST_SUITE(ConvertersTest);
	CPPUNIT_TEST(testString);
	CPPUNIT_TEST(testRanges);
	CPPUNIT_TEST(testIntegers);
	CPPUNIT_TEST_EXCEPTION(testBadValue, utils::ConvertError);
	CPPUNIT_TEST_EXCEPTION(testOverflow, utils::ConvertError);
	CPPUNIT_TEST_EXCEPTION(testEmptyToInteger, utils::ConvertError);
	CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConvertersTest, "ConvertersTest");
CPPUNIT_REGISTRY_ADD("ConvertersTest", "dependent");

void
ConvertersTest::testString() {
	using namespace utils;
	CPPUNIT_ASSERT_EQUAL(std::string("123"), toString(std::string("123")));
	CPPUNIT_ASSERT_EQUAL(std::string("123"), fromString<std::string>(std::string("123")));
}

void
ConvertersTest::testRanges() {
	typedef NEXTWEB_MAKE_TYPE_LIST3(std::list<char>, std::vector<char>, utils::Range<char const*>) SequenceList;
	testRangesWith<SequenceList>();
}

void
ConvertersTest::testIntegers() {
	using namespace utils;
	testIntegersWith<SignedIntegerList>();
	testIntegersWith<UnsignedIntegerList>();
}

void
ConvertersTest::testBadValue() {
	using namespace utils;
	fromString<Int32>("test test test");
}

void
ConvertersTest::testOverflow() {
	using namespace utils;
	std::stringstream stream;
	Int32 value = static_cast<Int32>(std::numeric_limits<Int16>::max()) + 10;
	stream << value;
	fromString<Int16>(stream.str());
}

void
ConvertersTest::testEmptyToInteger() {
	using namespace utils;
	fromString<Int32>("");
}

template <typename List> void
ConvertersTest::testRangesWith() {
	testRangeConversionWith<typename List::Value>();
	testRangesWith<typename List::Next>();
}

template <> void
ConvertersTest::testRangesWith<utils::NullType>() {
}

template <typename List> void
ConvertersTest::testIntegersWith() {
	testIntegerConversionWith<typename List::Value>();
	testIntegersWith<typename List::Next>();
}

template <> void
ConvertersTest::testIntegersWith<utils::NullType>() {
}

template <typename X> void
ConvertersTest::testRangeConversionWith() {
	using namespace utils;

	X x = as<X>("123");
	Range<typename X::const_iterator> value(x.begin(), x.end());
	CPPUNIT_ASSERT_EQUAL(std::string("123"), toString(value));
}

template <typename X> void
ConvertersTest::testIntegerConversionWith() {
	using namespace utils;
	X value = static_cast<X>(100);
	std::string str = toString(value);
	CPPUNIT_ASSERT_EQUAL(value, fromString<X>(str));
}

}} // namespaces
