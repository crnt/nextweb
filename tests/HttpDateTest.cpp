#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/HttpUtils.hpp"

namespace nextweb { namespace tests {

class HttpDateTest : public CppUnit::TestFixture {
public:
	void testParse();
	void testFormat();

private:
	CPPUNIT_TEST_SUITE(HttpDateTest);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST(testFormat);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HttpDateTest, "HttpDateTest");
CPPUNIT_REGISTRY_ADD("HttpDateTest", "server");

void
HttpDateTest::testParse() {
	CPPUNIT_ASSERT_EQUAL(static_cast<std::time_t>(1170416178), parseHttpDate("Fri, 02 Feb 2007 11:36:18 GMT"));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::time_t>(1170416178), parseHttpDate("Friday, 02-Feb-07 11:36:18 GMT"));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::time_t>(1170416178), parseHttpDate("Fri Feb 2 11:36:18 2007"));
}

void
HttpDateTest::testFormat() {
	char buffer[256];
	std::time_t now = static_cast<std::time_t>(1170416178);
	printHttpDate(buffer, sizeof(buffer), now);
	CPPUNIT_ASSERT_EQUAL(std::string("Fri, 02 Feb 2007 11:36:18 GMT"), std::string(buffer));
}

}} // namespaces
