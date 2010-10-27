#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/utils/Functors.hpp"

namespace nextweb { namespace tests {

class FunctorsTest : public CppUnit::TestFixture {

public:
	void testCILess();
	void testCIEqual();

private:
	template <typename Sequence> void testCILessWith();
	template <typename Sequence> void testCIEqualWith();

	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;

private:
	CPPUNIT_TEST_SUITE(FunctorsTest);
	CPPUNIT_TEST(testCILess);
	CPPUNIT_TEST(testCIEqual);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FunctorsTest, "FunctorsTest");
CPPUNIT_REGISTRY_ADD("FunctorsTest", "dependent");

void
FunctorsTest::testCILess() {
	testCILessWith<CharList>();
	testCILessWith<CharVector>();
	testCILessWith<std::string>();
	testCILessWith<CharSequence>();
}

void
FunctorsTest::testCIEqual() {
	testCIEqualWith<CharList>();
	testCIEqualWith<CharVector>();
	testCIEqualWith<std::string>();
	testCIEqualWith<CharSequence>();
}

template <typename Sequence> void
FunctorsTest::testCILessWith() {
	using namespace utils;
	CILess<Sequence> checker;
	CPPUNIT_ASSERT_EQUAL(true, (checker(as<Sequence>("Abc"), as<Sequence>("BcD"))));
}

template <typename Sequence> void
FunctorsTest::testCIEqualWith() {
	using namespace utils;
	CIEqual<Sequence> checker;
	CPPUNIT_ASSERT_EQUAL(true, (checker(as<Sequence>("Abc"), as<Sequence>("aBc"))));
}

}} // namespaces
