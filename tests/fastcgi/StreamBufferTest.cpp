#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/fastcgi/impl/StreamBuffer.hpp"

namespace nextweb { namespace tests {

class StreamBufferTest : public CppUnit::TestFixture {

public:
	void testGetLine();
	void testPutBack();
	void testPosition();

private:
	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char*> CharBuffer;
	typedef utils::Range<char const*> CharSequence;

	template <typename Sequence> void testGetLineWith();
	template <typename Sequence> void testPutBackWith();
	template <typename Sequence> void testPositionWith();
	
private:
	CPPUNIT_TEST_SUITE(StreamBufferTest);
	CPPUNIT_TEST(testGetLine);
	CPPUNIT_TEST(testPutBack);
	CPPUNIT_TEST(testPosition);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(StreamBufferTest);

void
StreamBufferTest::testGetLine() {
	testGetLineWith<CharList>();
	testGetLineWith<CharBuffer>();
	testGetLineWith<CharVector>();
	testGetLineWith<std::string>();
	testGetLineWith<CharSequence>();
}

void
StreamBufferTest::testPutBack() {
	testPutBackWith<CharList>();
	testPutBackWith<CharBuffer>();
	testPutBackWith<CharVector>();
	testPutBackWith<std::string>();
	testPutBackWith<CharSequence>();
}

void
StreamBufferTest::testPosition() {
	testPositionWith<CharList>();
	testPositionWith<CharBuffer>();
	testPositionWith<CharVector>();
	testPositionWith<std::string>();
	testPositionWith<CharSequence>();
}

template <typename Sequence> void
StreamBufferTest::testGetLineWith() {

	char source[] = "123 456\n789 012\n345 678\n901 234\n567 890";
	Sequence seq = as<Sequence>(source);

	fastcgi::StreamBuffer<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::string line;
	std::istream stream(&buffer);
	
	std::size_t i = 0;
	char const* values[] = { "123 456", "789 012", "345 678", "901 234", "567 890" };
	
	CPPUNIT_ASSERT(stream.good());
	for (; std::getline(stream, line); ++i) {
		CPPUNIT_ASSERT_EQUAL(std::string(values[i]), line);
	}
	CPPUNIT_ASSERT(stream.eof());
	CPPUNIT_ASSERT_EQUAL(sizeof(values) / sizeof(char const*), i);
}

template <typename Sequence> void
StreamBufferTest::testPutBackWith() {

	char source[] = "123 456 012 345 678 901 234 567 890";
	Sequence seq = as<Sequence>(source);

	char value[8]; // sizeof("123 456")
	fastcgi::StreamBuffer<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::istream stream(&buffer);
	CPPUNIT_ASSERT(stream.good());
	
	stream.putback('#');
	CPPUNIT_ASSERT(stream.fail());
	stream.clear();
	
	memset(value, 0, sizeof(value));
	stream.get(value, sizeof(value));

	CPPUNIT_ASSERT(stream.good());
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streamsize>(sizeof(value) - 1), stream.gcount());
	CPPUNIT_ASSERT_EQUAL(std::string("123 456"), std::string(value));
	
	for (std::size_t i = 0; i < sizeof(value) - 1; ++i) {
		stream.unget();
		CPPUNIT_ASSERT(stream.good());
	}

	memset(value, 0, sizeof(value));
	stream.get(value, sizeof(value));

	CPPUNIT_ASSERT(stream.good());
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streamsize>(sizeof(value) - 1), stream.gcount());
	CPPUNIT_ASSERT_EQUAL(std::string("123 456"), std::string(value));
}

template <typename Sequence> void
StreamBufferTest::testPositionWith() {

	char source[] = "123 456 012 345 678 901 234 567 890";
	Sequence seq = as<Sequence>(source);
	
	char value[8]; // sizeof "123 456"
	fastcgi::StreamBuffer<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::istream stream(&buffer);
	stream.seekg(static_cast<std::streamoff>(8), std::ios::cur);
	CPPUNIT_ASSERT(stream.good());

	stream.get(value, sizeof(value));
	CPPUNIT_ASSERT(stream.good());
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streamsize>(sizeof(value) - 1), stream.gcount());
	CPPUNIT_ASSERT_EQUAL(std::string("012 345"), std::string(value));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streampos>(8 + sizeof(value) - 1), stream.tellg());
	
	stream.seekg(0);
	CPPUNIT_ASSERT(stream.good());
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streampos>(0), stream.tellg());
	
	stream.get(value, sizeof(value));
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streamsize>(sizeof(value) - 1), stream.gcount());
	CPPUNIT_ASSERT_EQUAL(std::string("123 456"), std::string(value));
	
	stream.seekg(static_cast<std::streamoff>(-2), std::ios::cur);
	CPPUNIT_ASSERT(stream.good());
	CPPUNIT_ASSERT_EQUAL(static_cast<std::streampos>(sizeof(value) - 3), stream.tellg());

	stream.seekg(static_cast<std::streamoff>(-2), std::ios::beg);
	CPPUNIT_ASSERT(stream.fail());
	stream.clear();

}

}} // namespaces
