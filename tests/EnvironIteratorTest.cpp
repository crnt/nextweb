#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "details/EnvironIterator.hpp"

namespace nextweb { namespace tests {

class EnvironIteratorTest : public CppUnit::TestFixture {

public:
	void testIterator();

private:
	CPPUNIT_TEST_SUITE(EnvironIteratorTest);
	CPPUNIT_TEST(testIterator);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EnvironIteratorTest, "EnvironIteratorTest");
CPPUNIT_REGISTRY_ADD("EnvironIteratorTest", "core");

void
EnvironIteratorTest::testIterator() {
	
	using namespace nextweb::details;
	
	char const* array[] = {
		"one", "two", "three", static_cast<char const*>(0)
	};

	EnvironIterator i(array), end;
	for (std::size_t count = 0; i != end; ++i, ++count) {
		CPPUNIT_ASSERT_EQUAL(array[count], *i);
	}
}

}} // namespaces
