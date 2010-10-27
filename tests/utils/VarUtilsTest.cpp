#include "acsetup.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "nextweb/Error.hpp"
#include "nextweb/utils/VarUtils.hpp"

namespace nextweb { namespace tests {

class VarUtilsTest : public CppUnit::TestFixture {

public:
	void testBadFormat();
	void testNonexistent();
	void testEmptyString();
	void testSubstitution();

private:
	CPPUNIT_TEST_SUITE(VarUtilsTest);
	CPPUNIT_TEST(testEmptyString);
	CPPUNIT_TEST(testSubstitution);
	CPPUNIT_TEST_EXCEPTION(testBadFormat, Error);
	CPPUNIT_TEST_EXCEPTION(testNonexistent, Error);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VarUtilsTest, "VarUtilsTest");
CPPUNIT_REGISTRY_ADD("VarUtilsTest", "plain");

void
VarUtilsTest::testBadFormat() {
	std::string test("x${a}${b}${");
	std::map<std::string, std::string> m;
	utils::substVars(test, m);
}

void
VarUtilsTest::testNonexistent() {
	std::string test("x${abc${def}}");
	std::map<std::string, std::string> m;
	m.insert(std::pair<std::string, std::string>("abc", "zyx"));
	m.insert(std::pair<std::string, std::string>("def", "wvu"));
	utils::substVars(test, m);
}

void
VarUtilsTest::testEmptyString() {
	std::map<std::string, std::string> m;
	CPPUNIT_ASSERT_EQUAL(std::string(), utils::substVars(std::string(), m));
}

void
VarUtilsTest::testSubstitution() {
	std::string test("x${abc${def}}");
	std::map<std::string, std::string> m;
	m.insert(std::pair<std::string, std::string>("abcwvu", "zyx"));
	m.insert(std::pair<std::string, std::string>("def", "wvu"));
	CPPUNIT_ASSERT_EQUAL(std::string("xzyx"), utils::substVars(test, m));
}

}} // namespaces
