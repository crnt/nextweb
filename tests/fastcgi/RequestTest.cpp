#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockIO.hpp"
#include "nextweb/fastcgi/impl/GenericRequest.hpp"

namespace nextweb { namespace tests {

class RequestTest : public CppUnit::TestFixture {

public:
	void testGet();
	void testPostN();
	void testPostRN();
	void testLargePost();

private:
	CPPUNIT_TEST_SUITE(RequestTest);
	CPPUNIT_TEST(testGet);
	// CPPUNIT_TEST(testPostN);
	// CPPUNIT_TEST(testPostRN);
	// CPPUNIT_TEST(testLargePost);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RequestTest);

void
RequestTest::testGet() {
	
	MockIO io;
	io.add("HTTPS=on");
	io.add("PATH_INFO=/");
	io.add("REQUEST_METHOD=GET");
	io.add("QUERY_STRING=x=abc%20def&y=test");
	io.checkIsValid();
	
	fastcgi::GenericRequest<MockIO> req(io, 1024);
	CPPUNIT_ASSERT_EQUAL(std::string("x=abc%20def&y=test"), req.getVar("QUERY_STRING"));
	CPPUNIT_ASSERT_EQUAL(std::string("test"), req.getArg("y"));
	CPPUNIT_ASSERT_EQUAL(std::string("abc def"), req.getArg("x"));
}

void
RequestTest::testPostN() {
	
	MockIO io;
	io.add("REQUEST_METHOD=POST");
	io.add("HTTP_CONTENT_LENGTH=1361");
	io.add("CONTENT_TYPE=multipart/form-data; boundary=\"---------------------------15403834263040891721303455736\"");
	io.attachFile("MultipartN.dat");
	fastcgi::GenericRequest<MockIO> req(io, 1024);
	
}

void
RequestTest::testPostRN() {
	
	MockIO io;
	io.add("REQUEST_METHOD=POST");
	io.add("HTTP_CONTENT_LENGTH=1361");
	io.attachFile("MultipartRN.dat");
	fastcgi::GenericRequest<MockIO> req(io, 1024);
}

void
RequestTest::testLargePost() {
}

}} // namespaces
