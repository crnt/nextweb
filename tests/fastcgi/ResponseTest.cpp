#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockIO.hpp"
#include "nextweb/fastcgi/impl/GenericResponse.hpp"

namespace nextweb { namespace tests {

class ResponseTest : public CppUnit::TestFixture {

public:
	void testHeaders();
	void testCookies();
	
private:
	CPPUNIT_TEST_SUITE(ResponseTest);
	CPPUNIT_TEST(testCookies);
	CPPUNIT_TEST(testHeaders);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ResponseTest);

void
ResponseTest::testHeaders() {

	using namespace fastcgi;
	
	MockIO io;
	GenericResponse<MockIO> response(io);
	response.setHttpStatus(HttpStatus::OK);
	response.setCookie(Cookie("test", "test-value"));
	response.write("12345", sizeof("12345") - 1);
}

void
ResponseTest::testCookies() {

	using namespace fastcgi;
	MockIO io;
	GenericResponse<MockIO> response(io);
}


}} // namespaces
