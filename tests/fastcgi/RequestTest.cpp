#include "acsetup.hpp"

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockIO.hpp"
#include "nextweb/fastcgi/HttpError.hpp"
#include "nextweb/fastcgi/impl/RequestImpl.hpp"
#include "nextweb/fastcgi/impl/GenericRequest.hpp"

namespace nextweb { namespace tests {

class RequestTest : public CppUnit::TestFixture {

public:
	void testGet();
	void testPost();
	void testMultipart();
	void testRequestImpl();
	void testBadMethod();
	void testBadMultipart();

private:
	CPPUNIT_TEST_SUITE(RequestTest);
	CPPUNIT_TEST(testGet);
	CPPUNIT_TEST(testPost);
	CPPUNIT_TEST(testMultipart);
	CPPUNIT_TEST(testRequestImpl);
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

	fastcgi::GenericRequest<MockIO> req(io, 1024);
	CPPUNIT_ASSERT_EQUAL(std::string("field"), req.getArg("field"));
	CPPUNIT_ASSERT_EQUAL(std::string("test-field"), req.getArg("another-field"));
	CPPUNIT_ASSERT_EQUAL(false, req.hasArg("nonexistent"));

	fastcgi::File file = req.getFile("file");
	CPPUNIT_ASSERT_EQUAL(std::string("install.tst"), file.name());
	fastcgi::File anotherFile = req.getFile("another-file");
	CPPUNIT_ASSERT_EQUAL(std::string("copying.tst"), anotherFile.name());
	CPPUNIT_ASSERT_EQUAL(false, req.hasFile("nonexistent"));
}

void
RequestTest::testRequestImpl() {

	using namespace fastcgi;

	MockIO io;
	io.add("HTTPS=on");
	io.add("PATH_INFO=/");
	io.add("REQUEST_METHOD=GET");
	io.add("QUERY_STRING=x=abc%20def&x=ghi%20jkl&y=test");
	
	std::size_t i;
	RequestImpl<MockIO> req(io, 1024);
	CPPUNIT_ASSERT_EQUAL(std::string("test"), req.getArg("y"));
	
	i = 0;
	char const* names[] = { "x", "y" }; 
	char const* patterns[] = { "abc def", "ghi jkl" }; 
	Enumeration<std::string const&>::Pointer x = req.getArgList("x");
	for (; x->hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(patterns[i]), x->nextElement());
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(patterns) / sizeof(char const*), i);
	
	i = 0;
	Enumeration<std::string const&>::Pointer n = req.getArgNames();
	for (; n->hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(names[i]), n->nextElement());
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(names) / sizeof(char const*), i);
}

void
RequestTest::testBadMethod() {

	MockIO io;
	io.add("METHOD=BAD");
	fastcgi::GenericRequest<MockIO> req(io, 1024);
}

void
RequestTest::testBadMultipart() {
	MockIO io;
	io.add("REQUEST_METHOD=POST");
	io.add("HTTP_CONTENT_LENGTH=5000");
	io.add("CONTENT_TYPE=multipart/form-data; boundary=---------------------------68126048419810193861179415823");
	io.attachFile("data/bad-multipart.tst");
	fastcgi::GenericRequest<MockIO> req(io, 1024);
}

}} // namespaces
