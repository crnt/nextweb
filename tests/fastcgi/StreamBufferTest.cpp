#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>
#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/fastcgi/impl/StreamBuffer.hpp"

namespace nextweb { namespace tests {

class StreamBufferTest : public CppUnit::TestFixture {

public:
	void testBuffer();

private:
	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;
	template <typename Sequence> void testBufferWith();
	
private:
	CPPUNIT_TEST_SUITE(StreamBufferTest);
	// CPPUNIT_TEST(testBuffer);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(StreamBufferTest);

void
StreamBufferTest::testBuffer() {
	testBufferWith<CharList>();
	testBufferWith<CharVector>();
	testBufferWith<std::string>();
	testBufferWith<CharSequence>();
}

template <typename Sequence> void
StreamBufferTest::testBufferWith() {

	char const* values[] = { "123 456", "789 012", "345 678", "901 234", "567 890" };
	
	Sequence seq = as<Sequence>("123 456\n 789 012\n 345 678\n 901 234\n 567 890");
	fastcgi::StreamBuffer<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::string line;
	std::istream stream(&buffer);
	
	std::size_t i = 0;
	CPPUNIT_ASSERT(stream.good());
	for (; std::getline(stream, line); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[i]), line);
	}
	CPPUNIT_ASSERT(stream.eof());
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), i);
}

}} // namespaces
