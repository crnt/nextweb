#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/SharedPtr.hpp"
#include "details/MultiThreaded.hpp"
#include "details/SingleThreaded.hpp"

namespace nextweb { namespace tests {

class SharedTest : public CppUnit::TestFixture {

public:
	void testSwap();
	void testConstruct();
	void testAssignment();

private:
	template <typename Threading> void testSwapWith();
	template <typename Threading> void testConstructWith();
	template <typename Threading> void testAssignmentWith();

private:
	CPPUNIT_TEST_SUITE(SharedTest);
	CPPUNIT_TEST(testSwap);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testAssignment);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SharedTest, "SharedTest");
CPPUNIT_REGISTRY_ADD("SharedTest", "core");

void
SharedTest::testSwap() {
	using namespace nextweb::details;
	testSwapWith<MultiThreaded>();
	testSwapWith<SingleThreaded>();
}

void
SharedTest::testConstruct() {
	using namespace nextweb::details;
	testConstructWith<MultiThreaded>();
	testConstructWith<SingleThreaded>();
}

void
SharedTest::testAssignment() {
	using namespace nextweb::details;
	testAssignmentWith<MultiThreaded>();
	testAssignmentWith<SingleThreaded>();
}

template <typename Threading> inline void
SharedTest::testSwapWith() {

	using namespace nextweb::details;
	typedef typename Threading::ReferenceCounted Type;
	SharedPtr<Type> ptr(new Type()), target(new Type());
	for (int i = 0; i < 10; ++i) {
		ptr.swap(target);
	}
}

template <typename Threading> inline void
SharedTest::testConstructWith() {

	using namespace nextweb::details;
	typedef typename Threading::ReferenceCounted Type;
	SharedPtr<Type> ptr(new Type());
	for (int i = 0; i < 10; ++i) {
		SharedPtr<Type> target(ptr);
	}
}

template <typename Threading> inline void
SharedTest::testAssignmentWith() {

	using namespace nextweb::details;
	typedef typename Threading::ReferenceCounted Type;
	SharedPtr<Type> ptr(new Type()), target(new Type());
	for (int i = 0; i < 10; ++i) {
		ptr = target;
	}
}

}} // namespaces
