#include "acsetup.hpp"

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/utils/Tuple.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace tests {

class TupleTest : public CppUnit::TestFixture {

public:
	void testFunctions();

private:
	CPPUNIT_TEST_SUITE(TupleTest);
	CPPUNIT_TEST(testFunctions);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TupleTest, "TupleTest");
CPPUNIT_REGISTRY_ADD("TupleTest", "dependent");

void
TupleTest::testFunctions() {

	using namespace utils;
	typedef NEXTWEB_MAKE_TUPLE3(void*, std::string, unsigned int) TestTuple;

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<TypeListNthItem<TestTuple, 0>::Type, void*>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<TypeListNthItem<TestTuple, 1>::Type, std::string>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<TypeListNthItem<TestTuple, 2>::Type, unsigned int>::RESULT));
	
	TestTuple tuple = makeTuple(reinterpret_cast<void*>(12345), std::string("12345"), static_cast<unsigned int>(12345));
	CPPUNIT_ASSERT_EQUAL(reinterpret_cast<void*>(12345), tupleGetItem<0>(tuple));
	CPPUNIT_ASSERT_EQUAL(std::string("12345"), tupleGetItem<1>(tuple));
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(12345), tupleGetItem<2>(tuple));
}

}} // namespaces
