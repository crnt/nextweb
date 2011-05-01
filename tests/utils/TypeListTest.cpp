#include "acsetup.hpp"

#include <complex>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/utils/TypeList.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace tests {

class TypeListTest : public CppUnit::TestFixture {
	
public:
	void testNthItem();
	void testIndexOf();
	void testTypeList();

private:
	CPPUNIT_TEST_SUITE(TypeListTest);
	CPPUNIT_TEST(testNthItem);
	CPPUNIT_TEST(testIndexOf);
	CPPUNIT_TEST(testTypeList);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TypeListTest, "TypeListTest");
CPPUNIT_REGISTRY_ADD("TypeListTest", "dependent");

void
TypeListTest::testNthItem() {

	using namespace utils;
	typedef NEXTWEB_MAKE_TYPE_LIST9(char, short, int, long, long long, float, double, std::complex<float>, std::complex<double>) MultiList;
	
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 0>::Type, char>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 1>::Type, short>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 2>::Type, int>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 3>::Type, long>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 4>::Type, long long>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 5>::Type, float>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 6>::Type, double>::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 7>::Type, std::complex<float> >::RESULT));
	CPPUNIT_ASSERT((IsSame<TypeListNthItem<MultiList, 8>::Type, std::complex<double> >::RESULT));
}

void
TypeListTest::testIndexOf() {

	using namespace utils;
	typedef NEXTWEB_MAKE_TYPE_LIST9(char, short, int, long, long long, float, double, std::complex<float>, std::complex<double>) MultiList;

	CPPUNIT_ASSERT_EQUAL(0, (TypeListIndexOf<MultiList, char>::RESULT));
	CPPUNIT_ASSERT_EQUAL(1, (TypeListIndexOf<MultiList, short>::RESULT));
	CPPUNIT_ASSERT_EQUAL(2, (TypeListIndexOf<MultiList, int>::RESULT));
	CPPUNIT_ASSERT_EQUAL(3, (TypeListIndexOf<MultiList, long>::RESULT));
	CPPUNIT_ASSERT_EQUAL(4, (TypeListIndexOf<MultiList, long long>::RESULT));
	CPPUNIT_ASSERT_EQUAL(5, (TypeListIndexOf<MultiList, float>::RESULT));
	CPPUNIT_ASSERT_EQUAL(6, (TypeListIndexOf<MultiList, double>::RESULT));
	CPPUNIT_ASSERT_EQUAL(7, (TypeListIndexOf<MultiList, std::complex<float> >::RESULT));
	CPPUNIT_ASSERT_EQUAL(8, (TypeListIndexOf<MultiList, std::complex<double> >::RESULT));
}

void
TypeListTest::testTypeList() {

	using namespace utils;
	
	typedef NEXTWEB_MAKE_TYPE_LIST2(int, double) IntDoubleList;
	typedef TypeList<int, TypeList<double, NullType>::Type> TargetType;
	CPPUNIT_ASSERT((IsSame<IntDoubleList, TargetType>::RESULT));
}

}} // namespaces
