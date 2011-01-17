#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/Enumeration.hpp"
#include "nextweb/utils/Integer.hpp"
#include "nextweb/utils/TypeList.hpp"
#include "nextweb/utils/ReturnType.hpp"
#include "nextweb/utils/TypeTraits.hpp"

namespace nextweb { namespace tests {

class ReturnTypeTest : public CppUnit::TestFixture {

public:
	void testInteger();
	void testPointer();
	void testSharedPtr();
	void testComplicated();
	
private:
	template <typename List> void testIdentityWith();
	template <typename List> void testConstReferenceWith();

private:
	CPPUNIT_TEST_SUITE(ReturnTypeTest);
	CPPUNIT_TEST(testInteger);
	CPPUNIT_TEST(testPointer);
	CPPUNIT_TEST(testSharedPtr);
	CPPUNIT_TEST(testComplicated);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ReturnTypeTest, "ReturnTypeTest");
CPPUNIT_REGISTRY_ADD("ReturnTypeTest", "dependent");

void
ReturnTypeTest::testInteger() {
	testIdentityWith<utils::SignedIntegerList>();
	testIdentityWith<utils::UnsignedIntegerList>();
}

void
ReturnTypeTest::testPointer() {
	typedef NEXTWEB_MAKE_TYPE_LIST3(std::string const*, std::list<char>*, std::vector<char> const*) TestList;
	testIdentityWith<TestList>();
}

void
ReturnTypeTest::testSharedPtr() {
	typedef Enumeration<int> IntEnumeration;
	typedef Enumeration<double> DoubleEnumeration;
	typedef Enumeration<std::string> StringEnumeration;
	typedef NEXTWEB_MAKE_TYPE_LIST3(SharedPtr<IntEnumeration>, SharedPtr<DoubleEnumeration>, SharedPtr<StringEnumeration>) TestList;
	testIdentityWith<TestList>();
}

void
ReturnTypeTest::testComplicated() {
	typedef NEXTWEB_MAKE_TYPE_LIST3(std::string, std::list<char>, std::vector<char>) TestList;
	testConstReferenceWith<TestList>();
}

template <typename List> void
ReturnTypeTest::testIdentityWith() {
	using namespace utils;
	typedef typename List::Value ValueType;
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<typename ReturnType<ValueType>::Type, ValueType>::RESULT));
	testIdentityWith<typename List::Next>();
}

template <> void
ReturnTypeTest::testIdentityWith<utils::NullType>() {
}

template <typename List> void
ReturnTypeTest::testConstReferenceWith() {
	using namespace utils;
	typedef typename List::Value ValueType;
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<typename ReturnType<ValueType>::Type, ValueType const&>::RESULT));
	testConstReferenceWith<typename List::Next>();
}

template <> void
ReturnTypeTest::testConstReferenceWith<utils::NullType>() {
}

}} // namespaces
