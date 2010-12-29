#include "acsetup.hpp"

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockIO.hpp"
#include "nextweb/fastcgi/HttpError.hpp"
#include "nextweb/fastcgi/impl/GenericRequest.hpp"

namespace nextweb { namespace tests {

class RequestTest : public CppUnit::TestFixture {

public:
	void testGet();
	void testPost();
	void testMultipart();
	void testHeaderReader();
	void testBadMethod();
	void testBadMultipart();

private:
	CPPUNIT_TEST_SUITE(RequestTest);
	CPPUNIT_TEST(testGet);
	CPPUNIT_TEST(testPost);
	CPPUNIT_TEST(testMultipart);
	CPPUNIT_TEST(testHeaderReader);
	CPPUNIT_TEST_EXCEPTION(testBadMethod, fastcgi::HttpError);
	CPPUNIT_TEST_EXCEPTION(testBadMultipart, fastcgi::HttpError);
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
	CPPUNIT_ASSERT_EQUAL(std::string("test"), req.getArg("y"));
	CPPUNIT_ASSERT_EQUAL(std::string("abc def"), req.getArg("x"));

}

void
RequestTest::testPost() {

	MockIO io;
	io.add("REQUEST_METHOD=POST");
	io.add("HTTP_CONTENT_LENGTH=1705");
	io.attachFile("data/post.tst");
	io.checkIsValid();
	
	fastcgi::GenericRequest<MockIO> req(io, 1024);
	CPPUNIT_ASSERT_EQUAL(std::string("field"), req.getArg("field"));
	CPPUNIT_ASSERT_EQUAL(std::string("test-field"), req.getArg("another-field"));
}

void
RequestTest::testMultipart() {

	MockIO io;
	io.add("REQUEST_METHOD=POST");
	io.add("HTTP_CONTENT_LENGTH=15000");
	io.add("CONTENT_TYPE=multipart/form-data; boundary=---------------------------68126048419810193861179415823");
	io.attachFile("data/multipart.tst");
	io.checkIsValid();

	fastcgi::GenericRequest<MockIO> req(io, 1024);
	CPPUNIT_ASSERT_EQUAL(std::string("field"), req.getArg("field"));
	CPPUNIT_ASSERT_EQUAL(std::string("test-field"), req.getArg("another-field"));
	
}

void
RequestTest::testHeaderReader() {
}

void
RequestTest::testBadMethod() {

	MockIO io;
	io.add("METHOD=BAD");
	io.checkIsValid();
	fastcgi::GenericRequest<MockIO> req(io, 1024);
}

void
RequestTest::testBadMultipart() {
}

}} // namespaces
