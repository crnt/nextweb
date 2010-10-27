#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/fastcgi/impl/HttpUtils.hpp"

namespace nextweb { namespace tests {

class VarNameTest : public CppUnit::TestFixture {

public:
	void testVarName();

private:
	CPPUNIT_TEST_SUITE(VarNameTest);
	CPPUNIT_TEST(testVarName);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(VarNameTest);

void
VarNameTest::testVarName() {
	CPPUNIT_ASSERT_EQUAL(std::string("HTTP_HOST"), fastcgi::makeVarName("Host"));
	CPPUNIT_ASSERT_EQUAL(std::string("HTTP_CONTENT_TYPE"), fastcgi::makeVarName("Content-Type"));
}

}} // namespaces
