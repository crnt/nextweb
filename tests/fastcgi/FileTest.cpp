#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/fastcgi/impl/IterFileImpl.hpp"

namespace nextweb { namespace tests {

class FileTest : public CppUnit::TestFixture {

public:
	void testPartN();
	void testPartRN();

private:
	CPPUNIT_TEST_SUITE(FileTest);
	CPPUNIT_TEST(testPartN);
	CPPUNIT_TEST(testPartRN);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FileTest);

void
FileTest::testPartN() {
}

void
FileTest::testPartRN() {
}

}} // namespaces
