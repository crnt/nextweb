#include "acsetup.hpp"

#include <map>
#include <list>
#include <vector>
#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/utils/EnumerationImpl.hpp"

namespace nextweb { namespace tests {

class EnumerationTest : public CppUnit::TestFixture {

public:
	void testMap();
	void testSequence();
	
private:
	typedef std::list<std::string> StringList;
	typedef std::vector<std::string> StringVector;
	template <typename Sequence> void testSequenceWith();

private:
	CPPUNIT_TEST_SUITE(EnumerationTest);
	CPPUNIT_TEST(testMap);
	CPPUNIT_TEST(testSequence);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EnumerationTest, "EnumerationTest");
CPPUNIT_REGISTRY_ADD("EnumerationTest", "plain");

void
EnumerationTest::testMap() {

	using namespace utils;
	typedef std::map<std::size_t, std::string> TestMap;
	
	TestMap m;	
	std::size_t i;
	char const* patterns[] = { "one", "two", "three", "four", "five" };
	
	for (std::size_t count = 0; count < sizeof(patterns) / sizeof(char const*); ++count) {
		m.insert(std::make_pair(count, std::string(patterns[count])));
	}
	
	i = 0;
	Enumeration<TestMap::value_type const&>::Pointer en = makeIterEnumeration(m.begin(), m.end());
	for (; en->hasMoreElements(); ++i) {
		TestMap::value_type const& val = en->nextElement();
		CPPUNIT_ASSERT_EQUAL(i, val.first);
		CPPUNIT_ASSERT_EQUAL(std::string(patterns[i]), val.second);
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(patterns) / sizeof(char const*), i);
	
	i = 0;
	Enumeration<TestMap::key_type const&>::Pointer keys = makeSelectFirstEnumeration(m.begin(), m.end());
	for (; keys->hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(i, keys->nextElement());
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(patterns) / sizeof(char const*), i);
	
	i = 0;
	Enumeration<TestMap::mapped_type const&>::Pointer values = makeSelectSecondEnumeration(m.begin(), m.end());
	for (; values->hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(patterns[i]), values->nextElement());
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(patterns) / sizeof(char const*), i);
}

void
EnumerationTest::testSequence() {
	testSequenceWith<StringList>();
	testSequenceWith<StringVector>();
}

template <typename Sequence> void
EnumerationTest::testSequenceWith() {
	
	using namespace utils;
	
	Sequence seq;
	char const* values[] = { "one", "two", "three", "four", "five" };
	for (std::size_t i = 0; i < sizeof(values) / sizeof(char const*); ++i) {
		seq.push_back(values[i]);
	}
	typename Enumeration<typename Sequence::value_type const&>::Pointer en = makeIterEnumeration(seq.begin(), seq.end());
	
	std::size_t i = 0;
	for (; en->hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[i]), en->nextElement());
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), i);
}

}} // namespaces
