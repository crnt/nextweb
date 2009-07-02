#include "acsetup.hpp"

#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "details/Range.hpp"
#include "details/StringUtils.hpp"

namespace nextweb { namespace tests {

class SplitTest : public CppUnit::TestFixture {

public:
	void testSplit();
	void testSplitFirstOf();

private:
	typedef std::vector<char> BytesType;
	typedef details::Range<char const*> RangeType;

	template <typename Cont> void testSplitWith();
	template <typename Cont> void testSplitFirstOfWith();

private:
	CPPUNIT_TEST_SUITE(SplitTest);
	CPPUNIT_TEST(testSplit);
	CPPUNIT_TEST(testSplitFirstOf);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SplitTest, "SplitTest");
CPPUNIT_REGISTRY_ADD("SplitTest", "core");

void
SplitTest::testSplit() {
	testSplitWith<BytesType>();
	testSplitWith<RangeType>();
	testSplitWith<std::string>();
}

void
SplitTest::testSplitFirstOf() {
	testSplitFirstOfWith<BytesType>();
	testSplitFirstOfWith<RangeType>();
	testSplitFirstOfWith<std::string>();
}

template <typename Cont> inline void
SplitTest::testSplitWith() {
	
	using namespace nextweb::details;
	Cont value, head, tail;
	value = as<Cont>("best test best test best");
	while (!value.empty()) {
		split(value, ' ', head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename Cont::size_type>(4), head.size());
		value = trim(tail);
	}
	
	value = as<Cont>("best test best test best");
	while (!value.empty()) {
		split(value, "test", head, tail);
		CPPUNIT_ASSERT_EQUAL(makeString(as<Cont>("best")), makeString(trim(head)));
		value = trim(tail);
	}
}

template <typename Cont> inline void
SplitTest::testSplitFirstOfWith() {

	using namespace nextweb::details;
	Cont value, head, tail;
	value = as<Cont>("a&b;c&d;e&f");
	while (!value.empty()) {
		splitFirstOf(value, "&;", head, tail);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename Cont::size_type>(1), head.size());
		value = trim(tail);
	}
}

}} // namespaces
