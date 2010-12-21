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

	using namespace utils;
	using namespace templates;
	
	ParamSet params;
	for (std::size_t i = 0; i < 10; ++i) {
		params["data"][i] = i;
	}
	SubParam p = params["data"];
	for (std::size_t i = 0; i < 10; ++i) {
		std::stringstream stream;
		stream << "data." << i;
		CPPUNIT_ASSERT_EQUAL(utils::toString(i), params.get(stream.str()));
		CPPUNIT_ASSERT_EQUAL(utils::toString(i), p[i].as<std::string>());
	}
}

}} // namespaces
	