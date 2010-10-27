#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/fastcgi/impl/UrlEncode.hpp"

namespace nextweb { namespace tests {

class UrlEncodeTest : public CppUnit::TestFixture {

public:
	void testUrlencode();
	void testUrlencodeEmpty();
	void testUrlencodeLatin();
	void testUrldecode();
	void testUrldecodeEmpty();
	void testUrldecodeLatin();
	void testUrldecodeBadSuffix();

private:
	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;
	
	template <typename Sequence> void testUrlencodeWith();
	template <typename Sequence> void testUrlencodeEmptyWith();
	template <typename Sequence> void testUrlencodeLatinWith();
	template <typename Sequence> void testUrldecodeWith();
	template <typename Sequence> void testUrldecodeEmptyWith();
	template <typename Sequence> void testUrldecodeLatinWith();
	template <typename Sequence> void testUrldecodeBadSuffixWith();

private:
	CPPUNIT_TEST_SUITE(UrlEncodeTest);
	CPPUNIT_TEST(testUrlencode);
	CPPUNIT_TEST(testUrlencodeEmpty);
	CPPUNIT_TEST(testUrlencodeLatin);
	CPPUNIT_TEST(testUrldecode);
	CPPUNIT_TEST(testUrldecodeEmpty);
	CPPUNIT_TEST(testUrldecodeLatin);
	CPPUNIT_TEST(testUrldecodeBadSuffix);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(UrlEncodeTest);

void 
UrlEncodeTest::testUrlencode() {
	testUrlencodeWith<CharList>();
	testUrlencodeWith<CharVector>();
	testUrlencodeWith<std::string>();
	testUrlencodeWith<CharSequence>();
}
void
UrlEncodeTest::testUrlencodeEmpty() {
	testUrlencodeEmptyWith<CharList>();
	testUrlencodeEmptyWith<CharVector>();
	testUrlencodeEmptyWith<std::string>();
	testUrlencodeEmptyWith<CharSequence>();
}

void
UrlEncodeTest::testUrlencodeLatin() {
	testUrlencodeLatinWith<CharList>();
	testUrlencodeLatinWith<CharVector>();
	testUrlencodeLatinWith<std::string>();
	testUrlencodeLatinWith<CharSequence>();
}

void
UrlEncodeTest::testUrldecode() {
	testUrldecodeWith<CharList>();
	testUrldecodeWith<CharVector>();
	testUrldecodeWith<std::string>();
	testUrldecodeWith<CharSequence>();
}

void 
UrlEncodeTest::testUrldecodeEmpty() {
	testUrldecodeEmptyWith<CharList>();
	testUrldecodeEmptyWith<CharVector>();
	testUrldecodeEmptyWith<std::string>();
	testUrldecodeEmptyWith<CharSequence>();
}

void
UrlEncodeTest::testUrldecodeLatin() {
	testUrldecodeLatinWith<CharList>();
	testUrldecodeLatinWith<CharVector>();
	testUrldecodeLatinWith<std::string>();
	testUrldecodeLatinWith<CharSequence>();
}

void
UrlEncodeTest::testUrldecodeBadSuffix() {
	testUrldecodeBadSuffixWith<CharList>();
	testUrldecodeBadSuffixWith<CharVector>();
	testUrldecodeBadSuffixWith<std::string>();
	testUrldecodeBadSuffixWith<CharSequence>();
}

template <typename Sequence> inline void
UrlEncodeTest::testUrlencodeWith() {

	using namespace fastcgi;
	Sequence seq = as<Sequence>("раз два три четыре пять");
	std::string res = urlencode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string("%D2%C1%DA%20%C4%D7%C1%20%D4%D2%C9%20%DE%C5%D4%D9%D2%C5%20%D0%D1%D4%D8"), res);
}

template <typename Sequence> inline void
UrlEncodeTest::testUrlencodeEmptyWith() {

	using namespace fastcgi;
	Sequence seq;
	std::string res = urlencode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string(""), res);
}

template <typename Sequence> inline void
UrlEncodeTest::testUrlencodeLatinWith() {

	using namespace fastcgi;
	Sequence seq = as<Sequence>("abcd efgh");
	std::string res = urlencode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string("abcd%20efgh"), res);
}

template <typename Sequence> inline void
UrlEncodeTest::testUrldecodeWith() {
	
	using namespace fastcgi;
	Sequence seq = as<Sequence>("%D2%C1%DA%20%C4%D7%C1%20%D4%D2%C9%20%DE%C5%D4%D9%D2%C5%20%D0%D1%D4%D8");
	std::string res = urldecode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string("раз два три четыре пять"), res);
}

template <typename Sequence> inline void
UrlEncodeTest::testUrldecodeEmptyWith() {
	
	using namespace fastcgi;
	Sequence seq;
	std::string res = urldecode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string(""), res);
}

template <typename Sequence> inline void
UrlEncodeTest::testUrldecodeLatinWith() {
	
	using namespace fastcgi;
	Sequence seq = as<Sequence>("abcd%20efgh");
	std::string res = urldecode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string("abcd efgh"), res);
}

template <typename Sequence> inline void
UrlEncodeTest::testUrldecodeBadSuffixWith() {
	
	using namespace fastcgi;
	Sequence seq = as<Sequence>("abcd%20efgh%");
	std::string res = urldecode<std::string, Sequence>(seq);
	CPPUNIT_ASSERT_EQUAL(std::string("abcd efgh%"), res);
}

}} // namespaces
