#include "acsetup.hpp"

#include <string>
#include <sstream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/templates/ParamSet.hpp"
#include "nextweb/utils/StringConverters.hpp"

namespace nextweb { namespace tests {

class ParamsTest : public CppUnit::TestFixture {

public:
	void testOperations();

private:
	CPPUNIT_TEST_SUITE(ParamsTest);
	CPPUNIT_TEST(testOperations);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ParamsTest);

void
ParamsTest::testOperations() {

}

}} // namespaces
	