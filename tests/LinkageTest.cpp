#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/Server.hpp"

namespace nextweb { namespace tests {

class LinkageTest : public CppUnit::TestFixture {

public:
	void testServer();

private:
	CPPUNIT_TEST_SUITE(LinkageTest);
	CPPUNIT_TEST(testServer);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LinkageTest, "LinkageTest");
CPPUNIT_REGISTRY_ADD("LinkageTest", "core");

void
LinkageTest::testServer() {
	Server server;
}

}} // namespaces
