#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <string>
#include <vector>

#include "details/Range.hpp"

namespace nextweb { namespace tests {

class RangeTest : public CppUnit::TestFixture {

public:
	void testRange();
	
private:
	template <typename Cont> void testRangeWith();

private:
	CPPUNIT_TEST_SUITE(RangeTest);
	CPPUNIT_TEST(testRange);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RangeTest, "RangeTest");
CPPUNIT_REGISTRY_ADD("RangeTest", "core");

void
RangeTest::testRange() {
	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	testRangeWith<CharList>();
	testRangeWith<CharVector>();
	testRangeWith<std::string>();
	testRangeWith<std::wstring>();
}

template <typename Cont> void
RangeTest::testRangeWith() {

	using namespace nextweb::details;
	
	Range<char const*> range;
	CPPUNIT_ASSERT(range.empty());
	CPPUNIT_ASSERT_EQUAL(static_cast<Range<char const*>::size_type>(0), range.size());

	char const str[] = "test string";
	
	range = makeRange(str);
	CPPUNIT_ASSERT(!range.empty());
	CPPUNIT_ASSERT_EQUAL(static_cast<Range<char const*>::size_type>(strlen(str)), range.size());
	
	Cont container(range.begin(), range.end());
	CPPUNIT_ASSERT_EQUAL(container.size(), static_cast<typename Cont::size_type>(range.size()));

	Range<typename Cont::const_iterator> result(container.begin(), container.end());
	CPPUNIT_ASSERT_EQUAL(range.size(), result.size());

	CPPUNIT_ASSERT(range == result);
	CPPUNIT_ASSERT(range <= result);
	CPPUNIT_ASSERT(range >= result);
}

}} // namespaces
