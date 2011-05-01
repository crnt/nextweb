#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/Error.hpp"
#include "nextweb/fastcgi/Cookie.hpp"

namespace nextweb { namespace tests {

class CookieTest : public CppUnit::TestFixture {

public:
	void testValue();
	void testBadDate();
	void testEmptyName();

private:
	CPPUNIT_TEST_SUITE(CookieTest);
	CPPUNIT_TEST(testValue);
	CPPUNIT_TEST(testBadDate);
	CPPUNIT_TEST_EXCEPTION(testEmptyName, Error);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CookieTest);

void
CookieTest::testValue() {
	using namespace fastcgi;
	Cookie cookie("name", "value");
}

void
CookieTest::testBadDate() {
}

void
CookieTest::testEmptyName() {
	using namespace fastcgi;
	Cookie cookie("", "value");
	cookie.str();
}

}} // namespaces
