#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/fastcgi/impl/LineReader.hpp"

namespace nextweb { namespace tests {

class LineReaderTest : public CppUnit::TestFixture {

public:
	void testReadLines();
	void testReadMultilines();

private:
	template <typename Sequence> void testReadLinesWith();
	template <typename Sequence> void testReadMultilinesWith();

	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;

private:
	CPPUNIT_TEST_SUITE(LineReaderTest);
	CPPUNIT_TEST(testReadLines);
	CPPUNIT_TEST(testReadMultilines);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(LineReaderTest);

void
LineReaderTest::testReadLines() {
	testReadLinesWith<CharList>();
	testReadLinesWith<CharVector>();
	testReadLinesWith<std::string>();
	testReadLinesWith<CharSequence>();
}

void
LineReaderTest::testReadMultilines() {
	testReadMultilinesWith<CharList>();
	testReadMultilinesWith<CharVector>();
	testReadMultilinesWith<std::string>();
	testReadMultilinesWith<CharSequence>();
}

template <typename Sequence> void
LineReaderTest::testReadLinesWith() {
	
	char const* values[] = { "123", "456", "789" };
	Sequence const value = as<Sequence>("123\n456\n789");
	
	std::size_t i = 0;
	fastcgi::LineReader<Sequence> reader(value);
	for (; reader.hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[i]), makeString(reader.nextElement()));
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), i);
}

template <typename Sequence> void
LineReaderTest::testReadMultilinesWith() {
	
	char const* values[] = { "123\n 123\n 123\n 123", "456", "789" };
	Sequence const value = as<Sequence>("123\n 123\n 123\n 123\n456\n789");

	std::size_t i = 0;
	fastcgi::LineReader<Sequence> reader(value);
	for (; reader.hasMoreElements(); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[i]), makeString(reader.nextElement()));
	}
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), i);
}

}} // namespaces
