#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockShared.hpp"
#include "nextweb/SharedPtr.hpp"

namespace nextweb { namespace tests {

class SharedPtrTest : public CppUnit::TestFixture {

public:
	void testAssign();
	void testCompare();
	void testCopyConstruct();
	void testDefaultConstruct();

private:
	CPPUNIT_TEST_SUITE(SharedPtrTest);
	CPPUNIT_TEST(testAssign);
	CPPUNIT_TEST(testCompare);
	CPPUNIT_TEST(testCopyConstruct);
	CPPUNIT_TEST(testDefaultConstruct);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SharedPtrTest, "SharedPtrTest");
CPPUNIT_REGISTRY_ADD("SharedPtrTest", "plain");

void
SharedPtrTest::testAssign() {
	
	SharedPtr<MockShared> s(new MockShared());
	SharedPtr<MockSharedDerived> d(new MockSharedDerived());
	MockShared *ps = s.get();
	MockSharedDerived *pd = d.get();

	CPPUNIT_ASSERT_EQUAL(true, MockShared::isLive(ps));
	CPPUNIT_ASSERT_EQUAL(true, MockShared::isLive(pd));
	
	s = d;
	CPPUNIT_ASSERT_EQUAL(true, MockShared::isLive(pd));
	CPPUNIT_ASSERT_EQUAL(false, MockShared::isLive(ps));
}

void
SharedPtrTest::testCompare() {

	SharedPtr<MockShared> s(new MockShared());
	SharedPtr<MockSharedDerived> d(new MockSharedDerived());
	MockShared *ps = s.get();
	MockSharedDerived *pd = d.get();

	CPPUNIT_ASSERT_EQUAL(true, s == ps);
	CPPUNIT_ASSERT_EQUAL(true, s != pd);
	CPPUNIT_ASSERT_EQUAL(true, d != ps);

	CPPUNIT_ASSERT_EQUAL(true, MockShared::isLive(ps));
	CPPUNIT_ASSERT_EQUAL(true, MockShared::isLive(pd));
}

void
SharedPtrTest::testCopyConstruct() {

	SharedPtr<MockSharedDerived> s(new MockSharedDerived());
	SharedPtr<MockShared> d(s);
	CPPUNIT_ASSERT_EQUAL(true, s == d);
	SharedPtr<MockSharedDerived> o = d.cast<MockSharedDerived>();
	CPPUNIT_ASSERT_EQUAL(true, s == o);
}

void
SharedPtrTest::testDefaultConstruct() {

	SharedPtr<MockShared> s;
	CPPUNIT_ASSERT(!s);
	CPPUNIT_ASSERT_EQUAL(false, MockShared::isLive(s.get()));
	
	s.reset(new MockShared());
	CPPUNIT_ASSERT(s);
	CPPUNIT_ASSERT_EQUAL(true, MockShared::isLive(s.get()));
}

}} // namespaces
