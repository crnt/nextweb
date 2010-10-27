#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/utils/Integer.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace tests {

class IntegerTest : public CppUnit::TestFixture {

public:
	void testTypedefs();
	void testMakeSigned();
	void testMakeUnsigned();

private:
	CPPUNIT_TEST_SUITE(IntegerTest);
	CPPUNIT_TEST(testTypedefs);
	CPPUNIT_TEST(testMakeSigned);
	CPPUNIT_TEST(testMakeUnsigned);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntegerTest, "IntegerTest");
CPPUNIT_REGISTRY_ADD("IntegerTest", "dependent");

void
IntegerTest::testTypedefs() {
	
	using namespace utils;
	
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), sizeof(Int8));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), sizeof(UInt8));

	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(2), sizeof(Int16));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(2), sizeof(UInt16));

	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(4), sizeof(Int32));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(4), sizeof(UInt32));

	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(8), sizeof(Int64));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(8), sizeof(UInt64));
}

void
IntegerTest::testMakeSigned() {
	
	using namespace utils;
	
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<signed char, MakeSigned<char>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<signed char, MakeSigned<unsigned char>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<short, MakeSigned<short>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<short, MakeSigned<unsigned short>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<int, MakeSigned<int>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<int, MakeSigned<unsigned int>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<long, MakeSigned<long>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<long, MakeSigned<unsigned long>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<long long, MakeSigned<long long>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<long long, MakeSigned<unsigned long long>::Type>::RESULT));
}

void
IntegerTest::testMakeUnsigned() {

	using namespace utils;
	
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned char, MakeUnsigned<char>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned char, MakeUnsigned<unsigned char>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned short, MakeUnsigned<short>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned short, MakeUnsigned<unsigned short>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned int, MakeUnsigned<int>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned int, MakeUnsigned<unsigned int>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned long, MakeUnsigned<long>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned long, MakeUnsigned<unsigned long>::Type>::RESULT));

	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned long long, MakeUnsigned<long long>::Type>::RESULT));
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<unsigned long long, MakeUnsigned<unsigned long long>::Type>::RESULT));
}

}} // namespace
