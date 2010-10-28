#include "acsetup.hpp"

#include <string>
#include <cstdlib>
#include <cstring>
#include <numeric>

#include <unistd.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MockMappedFileImpl.hpp"
#include "nextweb/utils/MappedFile.hpp"
#include "nextweb/utils/SystemError.hpp"

namespace nextweb { namespace tests {

class MappedFileTest : public CppUnit::TestFixture {

public:
	void testReadFile();
	void testWriteFile();
	void testReadNonExistent();
	void testWriteNonExistent();
	
private:
	void unlinkIfExists(std::string const &name);
	template <typename Char> void testReadFileWith();
	template <typename Char> void testWriteFileWith();
	template <typename Char> std::size_t setupStream(std::string const &name, std::size_t count);
	
private:
	CPPUNIT_TEST_SUITE(MappedFileTest);
	CPPUNIT_TEST(testReadFile);
	CPPUNIT_TEST(testWriteFile);
	CPPUNIT_TEST(testWriteNonExistent);
	CPPUNIT_TEST_EXCEPTION(testReadNonExistent, utils::SystemError);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MappedFileTest, "MappedFileTest");
CPPUNIT_REGISTRY_ADD("MappedFileTest", "dependent");

void
MappedFileTest::testReadFile() {
	testReadFileWith<char>();
	testReadFileWith<wchar_t>();	
}

void
MappedFileTest::testWriteFile() {
	testWriteFileWith<char>();
	testWriteFileWith<wchar_t>();	
}

void
MappedFileTest::testReadNonExistent() {
	
	using namespace utils;
	MockMappedFileImpl<MappedFileImpl> impl("read.dat", true, 0);
}

void
MappedFileTest::testWriteNonExistent() {
	
	using namespace utils;
	
	typedef MockMappedFileImpl<MappedFileImpl> ImplType;
	typedef WriteableMappedFileIterator<char, ImplType> IteratorType;
	
	SharedPtr<ImplType> impl(new ImplType("impl.dat", false, 0));
	for (IteratorType i = IteratorType(0, impl), end = IteratorType(impl->size(), impl); i != end; ++i) {
	}
}

template <typename Char> void
MappedFileTest::testReadFileWith() {
	
	using namespace utils;
	std::stringstream stream;
	stream << "read" << sizeof(Char) << ".dat";
	
	unlinkIfExists(stream.str());
	std::size_t checksum = setupStream<Char>(stream.str(), 1000);
	
	typedef MockMappedFileImpl<MappedFileImpl> ImplType;
	typedef ReadableMappedFile<char, ImplType> FileType;
	
	std::size_t size = 0;
	FileType file(stream.str().c_str(), 0);
	for (FileType::size_type i = 0; i < file.size(); ++i) {
		size += static_cast<Char>(file.at(i));
	}
	std::size_t otherSize = std::accumulate(file.begin(), file.end(), static_cast<std::size_t>(0));
	std::size_t reversedSize = std::accumulate(file.rbegin(), file.rend(), static_cast<std::size_t>(0));
	// CPPUNIT_ASSERT_EQUAL(size, checksum);
	CPPUNIT_ASSERT_EQUAL(size, otherSize);
	CPPUNIT_ASSERT_EQUAL(size, reversedSize);
}

template <typename Char> void
MappedFileTest::testWriteFileWith() {

	using namespace utils;
	std::stringstream stream;
	stream << "write" << sizeof(Char) << ".dat";

	std::size_t checksum = setupStream<Char>(stream.str(), 10000);
	unlinkIfExists(stream.str());	

	typedef MockMappedFileImpl<MappedFileImpl> ImplType;
	typedef WriteableMappedFile<char, ImplType> FileType;
	
	std::size_t size = 0;
	FileType file(stream.str().c_str(), 0);
	for (FileType::size_type i = 0; i < file.size(); ++i) {
		size += static_cast<Char>(file.at(i));
	}
	std::size_t otherSize = std::accumulate(file.begin(), file.end(), static_cast<std::size_t>(0));
	std::size_t reversedSize = std::accumulate(file.rbegin(), file.rend(), static_cast<std::size_t>(0));
	// CPPUNIT_ASSERT_EQUAL(size, checksum);
	CPPUNIT_ASSERT_EQUAL(size, otherSize);
	CPPUNIT_ASSERT_EQUAL(size, reversedSize);

	std::random_shuffle(file.begin(), file.end());
	std::sort(file.begin(), file.end());
	
	std::size_t sortedSize = std::accumulate(file.rbegin(), file.rend(), static_cast<std::size_t>(0));
	CPPUNIT_ASSERT_EQUAL(size, sortedSize);
}

template <typename Char> std::size_t
MappedFileTest::setupStream(std::string const &name, std::size_t count) {

	std::size_t result = 0;
	std::ofstream stream(name.c_str());
	stream.exceptions(std::ios::badbit | std::ios::failbit);
	for (std::size_t i = 0; i < count; ++i) {
		Char value = static_cast<Char>(random());
		stream.write(reinterpret_cast<char*>(&value), sizeof(Char));
		result += value;
	}
	return result;
}

void
MappedFileTest::unlinkIfExists(std::string const &name) {
	unlink(name.c_str());
}

}} // namespaces
