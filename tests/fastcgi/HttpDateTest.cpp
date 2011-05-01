#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/Error.hpp"
#include "nextweb/fastcgi/HttpDate.hpp"

namespace nextweb { namespace tests {

class HttpDateTest : public CppUnit::TestFixture {

public:
	void testPeriod();
	void testParsing();
	void testPeriodParsing();
	
	void testBadPeriod();
	void testBadParsing();
	
	void testOperators();
	void testAsctimeFormat();
	void testRFC1123Format();
	void testRFC1036Format();

private:
	CPPUNIT_TEST_SUITE(HttpDateTest);
	CPPUNIT_TEST(testPeriod);
	CPPUNIT_TEST(testParsing);
	CPPUNIT_TEST(testPeriodParsing);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testAsctimeFormat);
	CPPUNIT_TEST(testRFC1123Format);
	CPPUNIT_TEST(testRFC1036Format);
	CPPUNIT_TEST_EXCEPTION(testBadPeriod, Error);
	CPPUNIT_TEST_EXCEPTION(testBadParsing, Error);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(HttpDateTest);

void
HttpDateTest::testPeriod() {
	
	using namespace fastcgi;
	HttpDate date(static_cast<std::time_t>(1170416178));
	CPPUNIT_ASSERT_EQUAL(HttpDate::fromString("Fri, 02 Feb 2007 11:36:18 GMT"), date);
	CPPUNIT_ASSERT_EQUAL(HttpDate::fromString("Fri, 02 Feb 2007 12:37:20 GMT"), date + std::string("1h1m2s"));
	date -= "1h1m2s";
	CPPUNIT_ASSERT_EQUAL(HttpDate::fromString("Fri, 02 Feb 2007 10:35:16 GMT"), date);
}

void
HttpDateTest::testParsing() {
	using namespace fastcgi;
	CPPUNIT_ASSERT_EQUAL(HttpDate(static_cast<std::time_t>(1170416178)), HttpDate::fromString("Fri, 02 Feb 2007 11:36:18 GMT"));
	CPPUNIT_ASSERT_EQUAL(HttpDate(static_cast<std::time_t>(1170416178)), HttpDate::fromString("Friday, 02-Feb-07 11:36:18 GMT"));
	CPPUNIT_ASSERT_EQUAL(HttpDate(static_cast<std::time_t>(1170416178)), HttpDate::fromString("Fri Feb 2 11:36:18 2007"));
	HttpDate::fromString("+2h 3m");
}

void
HttpDateTest::testPeriodParsing() {
	using namespace fastcgi;
	HttpDate date(static_cast<std::time_t>(1170416178));
	date += "+3m1d";
	date += "-20000h +30M";
	date += "2y";
}

void
HttpDateTest::testBadPeriod() {
	using namespace fastcgi;
	HttpDate::fromPeriod("123456");
}

void
HttpDateTest::testBadParsing() {
	using namespace fastcgi;
	HttpDate::fromString("1234567890");
}

void
HttpDateTest::testOperators() {
	using namespace fastcgi;
	CPPUNIT_ASSERT(HttpDate(static_cast<std::time_t>(1170416178)) == HttpDate::fromString("Fri Feb 2 11:36:18 2007"));
	CPPUNIT_ASSERT(HttpDate(static_cast<std::time_t>(1716140771)) != HttpDate::fromString("Fri Feb 2 11:36:18 2007"));
}

void
HttpDateTest::testAsctimeFormat() {
	using namespace fastcgi;
	HttpDate date(static_cast<std::time_t>(1170416178));
	CPPUNIT_ASSERT_EQUAL(std::string("Fri Feb 2 11:36:18 2007"), date.asAsctime());
}

void
HttpDateTest::testRFC1123Format() {
	using namespace fastcgi;
	HttpDate date(static_cast<std::time_t>(1170416178));
	CPPUNIT_ASSERT_EQUAL(std::string("Fri, 02 Feb 2007 11:36:18 GMT"), date.asRFC1123());
}

void
HttpDateTest::testRFC1036Format() {
	using namespace fastcgi;
	HttpDate date(static_cast<std::time_t>(1170416178));
	CPPUNIT_ASSERT_EQUAL(std::string("Friday, 02-Feb-07 11:36:18 GMT"), date.asRFC1036());
}

}} // namespaces
