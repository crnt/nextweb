#include "acsetup.hpp"

#include <limits>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "details/StringConverters.hpp"

#include "nextweb/Error.hpp"
#include "nextweb/StandardSetup.hpp"

namespace nextweb { namespace tests {

class ConvertorsTest : public CppUnit::TestFixture {

public:
	void testConversion();
	void testRangeError();
	void testBadConversion();
	void testEmptyConversion();

private:
	template <typename Basic> void testConversionWith();

private:
	CPPUNIT_TEST_SUITE(ConvertorsTest);
	CPPUNIT_TEST(testConversion);
	CPPUNIT_TEST_EXCEPTION(testRangeError, nextweb::Error);	
	CPPUNIT_TEST_EXCEPTION(testBadConversion, nextweb::Error);
	CPPUNIT_TEST_EXCEPTION(testEmptyConversion, nextweb::Error);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConvertorsTest, "ConvertorsTest");
CPPUNIT_REGISTRY_ADD("ConvertorsTest", "core");

void
ConvertorsTest::testConversion() {
	testConversionWith<short>();
	testConversionWith<unsigned short>();

	testConversionWith<int>();
	testConversionWith<unsigned int>();

	testConversionWith<long>();
	testConversionWith<unsigned long>();

	testConversionWith<long long>();
	testConversionWith<unsigned long long>();
}

void
ConvertorsTest::testRangeError() {
	
	using namespace nextweb::details;
	std::string rep = toString<StandardSetup>(std::numeric_limits<unsigned long>::max());
	fromString<StandardSetup, short>(rep);
}

void
ConvertorsTest::testBadConversion() {
	using namespace nextweb::details;
	fromString<StandardSetup, off_t>("gsdsqa");
}

void
ConvertorsTest::testEmptyConversion() {
	using namespace nextweb::details;
	fromString<StandardSetup, off_t>("");
}

template <typename Basic> void
ConvertorsTest::testConversionWith() {

	using namespace nextweb::details;
	char const* vars[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	for (Basic i = 0; i < 10; ++i) {
		std::string rep = toString<StandardSetup>(i);
		CPPUNIT_ASSERT_EQUAL(std::string(vars[i]), rep);
		Basic converted = fromString<StandardSetup, Basic>(vars[i]);
		CPPUNIT_ASSERT_EQUAL(i, converted);
	}

}

}} // namespaces
