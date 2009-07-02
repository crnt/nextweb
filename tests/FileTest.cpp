#include "acsetup.hpp"

#include <string>
#include <iostream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "details/ReadableFile.hpp"
#include "details/WriteableFile.hpp"

namespace nextweb { namespace tests {

template <typename Char>
struct Writer {
	std::size_t operator() (Char *buffer, std::size_t size) const;
};

class FileTest : public CppUnit::TestFixture {

public:
	void testWrite();
	void testReadable();
	void testWriteable();

private:
	CPPUNIT_TEST_SUITE(FileTest);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testReadable);
	CPPUNIT_TEST(testWriteable);
	CPPUNIT_TEST_SUITE_END();

private:
	template <typename Char> void testWriteWith();
	template <typename Char> void testWriteableWith();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FileTest, "FileTest");
CPPUNIT_REGISTRY_ADD("FileTest", "core");

template <typename Char> inline std::size_t
Writer<Char>::operator() (Char *buffer, std::size_t size) const {
	if (4096 / sizeof(Char) == size) {
		memset(buffer, 0, 1000 * sizeof(Char));
		return 1000 * sizeof(Char);
	}
	else {
		memset(buffer, 0, size * sizeof(Char));
		return size;
	}
}

void
FileTest::testWrite() {
	testWriteWith<wchar_t>();
	testWriteWith<unsigned char>();
	testWriteWith<unsigned short>();
}

void
FileTest::testReadable() {

	using namespace nextweb::details;
	
	std::string target;
	ReadableFile<char> file("data/get");
	std::copy(file.begin(), file.end(), std::back_inserter(target));
}

void
FileTest::testWriteable() {
	testWriteableWith<wchar_t>();
	testWriteableWith<unsigned char>();
	testWriteableWith<unsigned short>();
}

template <typename Char> void
FileTest::testWriteWith() {

	using namespace nextweb::details;
	
	std::stringstream stream;
	stream << "write-paging" << sizeof(Char) << ".tmp";
	WriteableFile<Char> file(stream.str().c_str(), 4096);
	
	Writer<Char> writer;
	file.fill(writer, 8 * 4096 / sizeof(Char));
}

template <typename Char> void
FileTest::testWriteableWith() {
	
	using namespace nextweb::details;
	
	std::stringstream stream;
	stream << "write-simple" << sizeof(Char) << ".tmp";
	WriteableFile<Char> file(stream.str().c_str());
	file.resize(1000);
	
	int count = 0;
	typedef typename WriteableFile<Char>::iterator Iterator;
	typedef typename std::iterator_traits<Iterator>::difference_type Difference;
	for (Iterator i = file.begin(), end = file.end(); i != end; ++i, ++count) {
		*i = static_cast<Char>(count);
	}
	std::random_shuffle(file.rbegin(), file.rend());
	std::sort(file.begin(), file.end());
	
	CPPUNIT_ASSERT_EQUAL(static_cast<typename WriteableFile<Char>::size_type>(1000),
		std::distance(file.begin(), file.end()));
	CPPUNIT_ASSERT_EQUAL(static_cast<Char>(0), 	file.at(0));
}

}} // namespaces
