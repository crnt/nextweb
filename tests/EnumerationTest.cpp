#include "acsetup.hpp"

#include <map>
#include <list>
#include <string>
#include <cstdio>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "details/EnumerationImpl.hpp"

namespace nextweb { namespace tests {

class EnumerationTest : public CppUnit::TestFixture {

public:
	void testStringMap();
	void testStringList();	
	
private:
	CPPUNIT_TEST_SUITE(EnumerationTest);
	CPPUNIT_TEST(testStringMap);
	CPPUNIT_TEST(testStringList);
	CPPUNIT_TEST_SUITE_END();
};  

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EnumerationTest, "EnumerationTest");
CPPUNIT_REGISTRY_ADD("EnumerationTest", "core");

void
EnumerationTest::testStringMap() {

	using namespace nextweb::details;
	typedef std::pair<std::string const, std::string> StringPair;
	
	char name[16], value[16];
	std::map<std::string, std::string> values;
	for (int i = 0; i < 10; ++i) {
		snprintf(name, sizeof(name), "name-%d", i);
		snprintf(value, sizeof(value), "value-%d", i);
		values.insert(std::make_pair<std::string, std::string>(name, value));
	}
	
	Enumeration<StringPair const&>::PtrType en = makeEnumeration(values);
	for (int i = 0; en->hasMoreElements(); ++i) {
		Enumeration<StringPair const&>::ValueType p = en->nextElement();
		snprintf(name, sizeof(name), "name-%d", i);
		CPPUNIT_ASSERT_EQUAL(std::string(name), p.first);
		snprintf(value, sizeof(value), "value-%d", i);
		CPPUNIT_ASSERT_EQUAL(std::string(value), p.second);
	}
}

void
EnumerationTest::testStringList() {

	using namespace nextweb::details;
	std::list<std::string> values;
	
	char value[16];
	for (int i = 0; i < 10; ++i) {
		snprintf(value, sizeof(value), "value-%d", i);
		values.push_back(std::string(value));
	}
	
	Enumeration<std::string const&>::PtrType en = makeEnumeration(values);
	while (en->hasMoreElements()) {
		en->nextElement();
	}
}

}} // namespaces
