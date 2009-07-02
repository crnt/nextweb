#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <vector>
#include <iterator>
#include <algorithm>

#include "MockIterator.hpp"

namespace nextweb { namespace tests {

class IteratorTest : public CppUnit::TestFixture {

public:
	void testRandomAccess();
	void testBidirectional();

private:
	CPPUNIT_TEST_SUITE(IteratorTest);
	CPPUNIT_TEST(testRandomAccess);
	CPPUNIT_TEST(testBidirectional);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IteratorTest, "IteratorTest");
CPPUNIT_REGISTRY_ADD("IteratorTest", "core");

void
IteratorTest::testRandomAccess() {

	std::vector<int> values;
	for (int i = 0; i < 10000; ++i) {
		values.push_back(i);
	}
	typedef MockIterator<std::vector<int>::iterator> Iter;
	
	std::random_shuffle(Iter(values.begin()), Iter(values.end()));
	std::sort(Iter(values.begin()), Iter(values.end()));
	
	int count = 0;
	for (Iter i(values.begin()), end(values.end()); i != end; ++i, ++count) {
	 	CPPUNIT_ASSERT_EQUAL(count, *i);
	}
	typedef std::reverse_iterator<Iter> Reverse;
	std::size_t result = std::count(Reverse(values.end()), Reverse(values.begin()), 1000);
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), result);
}

void
IteratorTest::testBidirectional() {

	std::list<int> values;
	for (int i = 0; i < 10000; ++i) {
		values.push_back(i);
	}
	typedef MockIterator<std::list<int>::iterator>  Iter;

	int count = 0;
	for (Iter i(values.begin()), end(values.end()); i != end; ++i, ++count) {
	 	CPPUNIT_ASSERT_EQUAL(count, *i);
	}
	typedef std::reverse_iterator<Iter> ReverseType;
	std::size_t result = std::count(ReverseType(values.end()), ReverseType(values.begin()), 1000);
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1), result);
}

}} // namespaces
