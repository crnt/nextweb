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
#include "nextweb/utils/TypeTraits.hpp"
#include "nextweb/utils/PassingType.hpp"

namespace nextweb { namespace tests {

class PassingTypeTest : public CppUnit::TestFixture {

public:
	void testInteger();
	void testPointer();
	void testSharedPtr();
	void testComplicated();
	
private:
	template <typename List> void testIdentityWith();
	template <typename List> void testConstReferenceWith();

private:
	CPPUNIT_TEST_SUITE(PassingTypeTest);
	CPPUNIT_TEST(testInteger);
	CPPUNIT_TEST(testPointer);
	CPPUNIT_TEST(testSharedPtr);
	CPPUNIT_TEST(testComplicated);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PassingTypeTest, "PassingTypeTest");
CPPUNIT_REGISTRY_ADD("PassingTypeTest", "dependent");

void
PassingTypeTest::testInteger() {
	testIdentityWith<utils::SignedIntegerList>();
	testIdentityWith<utils::UnsignedIntegerList>();
}

void
PassingTypeTest::testPointer() {
	typedef NEXTWEB_MAKE_TYPE_LIST3(std::string const*, std::list<char>*, std::vector<char> const*) TestList;
	testIdentityWith<TestList>();
}

void
PassingTypeTest::testSharedPtr() {
	typedef Enumeration<int> IntEnumeration;
	typedef Enumeration<double> DoubleEnumeration;
	typedef Enumeration<std::string> StringEnumeration;
	typedef NEXTWEB_MAKE_TYPE_LIST3(SharedPtr<IntEnumeration>, SharedPtr<DoubleEnumeration>, SharedPtr<StringEnumeration>) TestList;
	testIdentityWith<TestList>();
}

void
PassingTypeTest::testComplicated() {
	typedef NEXTWEB_MAKE_TYPE_LIST3(std::string, std::list<char>, std::vector<char>) TestList;
	testConstReferenceWith<TestList>();
}

template <typename List> void
PassingTypeTest::testIdentityWith() {
	using namespace utils;
	typedef typename List::Value ValueType;
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<typename PassingType<ValueType>::Type, ValueType>::RESULT));
	testIdentityWith<typename List::Next>();
}

template <> void
PassingTypeTest::testIdentityWith<utils::NullType>() {
}

template <typename List> void
PassingTypeTest::testConstReferenceWith() {
	using namespace utils;
	typedef typename List::Value ValueType;
	CPPUNIT_ASSERT_EQUAL(true, (IsSame<typename PassingType<ValueType>::Type, ValueType const&>::RESULT));
	testConstReferenceWith<typename List::Next>();
}

template <> void
PassingTypeTest::testConstReferenceWith<utils::NullType>() {
}

}} // namespaces
