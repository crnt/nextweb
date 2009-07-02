#include "acsetup.hpp"

#include <fstream>
#include <sstream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/Error.hpp"
#include "nextweb/StandardSetup.hpp"

#include "MockEnvironment.hpp"
#include "details/HttpConstants.hpp"
#include "details/RequestParams.hpp"

namespace nextweb { namespace tests {

class RequestTest : public CppUnit::TestFixture {

public:
	void testGet();
	void testEmpty();
	void testPostDisk();
	void testPostMemory();

private:
	template <typename Stream> void readFile(MockEnvironment<Stream> &env, char const *name);

private:
	CPPUNIT_TEST_SUITE(RequestTest);
	CPPUNIT_TEST(testGet);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testPostDisk);
	CPPUNIT_TEST(testPostMemory);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RequestTest, "RequestTest");
CPPUNIT_REGISTRY_ADD("RequestTest", "server");

void
RequestTest::testGet() {

	using namespace nextweb::details;
	
	RequestParams<StandardSetup> params;
	MockEnvironment<std::stringstream> environ(0);
	
	readFile(environ, "data/get");
	params.parse(environ, 1024);
	
	CPPUNIT_ASSERT_EQUAL(StandardSetup::StringType("/user"), params.getVar("SCRIPT_NAME"));
	CPPUNIT_ASSERT_EQUAL(StandardSetup::StringType("id=10&data=my%20data"), params.getVar("QUERY_STRING"));
	CPPUNIT_ASSERT_EQUAL(StandardSetup::StringType("/var/www/musicbay/htdocs"), params.getVar("DOCUMENT_ROOT"));
	CPPUNIT_ASSERT_EQUAL(StandardSetup::StringType("*/*"), params.getHttp("Accept"));
	
	CPPUNIT_ASSERT_EQUAL(StandardSetup::StringType("10"), params.getArg("id"));
	CPPUNIT_ASSERT_EQUAL(StandardSetup::StringType("my data"), params.getArg("data"));
}

void
RequestTest::testEmpty() {

	using namespace nextweb::details;
	RequestParams<StandardSetup> params;
	MockEnvironment<std::stringstream> environ(0);
	params.parse(environ, 1024);
}

void
RequestTest::testPostDisk() {

	using namespace nextweb::details;
	RequestParams<StandardSetup> params;
}

void
RequestTest::testPostMemory() {

	using namespace nextweb::details;
	RequestParams<StandardSetup> params;
}

template <typename Stream> inline void
RequestTest::readFile(MockEnvironment<Stream> &env, char const *name) {

	std::ifstream file;
	file.exceptions(std::ios::badbit);
	file.open(name);

	std::string line;
	while (std::getline(file, line)) {
		env.push_back(line);
	}
}

}} // namespaces

