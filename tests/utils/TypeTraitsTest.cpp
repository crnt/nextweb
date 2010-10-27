#include "acsetup.hpp"

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockTemplate.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace tests { 

class TypeTraitsTest : public CppUnit::TestFixture {

public:
	void testIsSame();
	void testIsConst();
	void testAddConst();
	void testRemoveConst();

private:
	CPPUNIT_TEST_SUITE(TypeTraitsTest);
	CPPUNIT_TEST(testIsSame);
	CPPUNIT_TEST(testIsConst);
	CPPUNIT_TEST(testAddConst);
	CPPUNIT_TEST(testRemoveConst);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TypeTraitsTest, "TypeTraitsTest");
CPPUNIT_REGISTRY_ADD("TypeTraitsTest", "plain");

void
TypeTraitsTest::testIsSame() {

	using namespace utils;

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<int, int>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<int const, int const>::RESULT));
	MockTemplate<IsSame<char, char>::RESULT> t;
	(void) t;
	
	CPPUNIT_ASSERT_EQUAL(false, (IsSame<int, double>::RESULT));
	CPPUNIT_ASSERT_EQUAL(false, (IsSame<int, int const>::RESULT));
	MockTemplate<IsSame<char, char const>::RESULT> f;
	(void) f;
}

void
TypeTraitsTest::testIsConst() {

	using namespace utils;

	CPPUNIT_ASSERT_EQUAL(true, IsConst<int const>::RESULT);
	CPPUNIT_ASSERT_EQUAL(true, IsConst<std::string const>::RESULT);
	MockTemplate<IsConst<char const>::RESULT> t;
	(void) t;
	
	CPPUNIT_ASSERT_EQUAL(false, IsConst<int>::RESULT);
	CPPUNIT_ASSERT_EQUAL(false, IsConst<std::string>::RESULT);
	MockTemplate<IsConst<char>::RESULT> f;
	(void) f;
}

void
TypeTraitsTest::testAddConst() {
	using namespace utils; 
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<AddConst<char>::Type, char const>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<AddConst<char const>::Type, char const>::RESULT));
}

void
TypeTraitsTest::testRemoveConst() {
	using namespace utils;
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<RemoveConst<char>::Type, char>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<RemoveConst<char const>::Type, char>::RESULT));
}

}} // namespaces
