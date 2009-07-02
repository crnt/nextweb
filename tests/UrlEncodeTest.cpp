#include "acsetup.hpp"

#include <vector>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "details/UrlEncode.hpp"

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
	typedef std::vector<char> BytesType;
	
	template <typename Cont> void testUrlencodeWith();
	template <typename Cont> void testUrlencodeEmptyWith();
	template <typename Cont> void testUrlencodeLatinWith();
	template <typename Cont> void testUrldecodeWith();
	template <typename Cont> void testUrldecodeEmptyWith();
	template <typename Cont> void testUrldecodeLatinWith();
	template <typename Cont> void testUrldecodeBadSuffixWith();

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

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UrlEncodeTest, "UrlEncodeTest");
CPPUNIT_REGISTRY_ADD("UrlEncodeTest", "core");

void 
UrlEncodeTest::testUrlencode() {
	testUrlencodeWith<BytesType>();
	testUrlencodeWith<std::string>();
}
void
UrlEncodeTest::testUrlencodeEmpty() {
	testUrlencodeEmptyWith<BytesType>();
	testUrlencodeEmptyWith<std::string>();
}

void
UrlEncodeTest::testUrlencodeLatin() {
	testUrlencodeLatinWith<BytesType>();
	testUrlencodeLatinWith<std::string>();
}

void
UrlEncodeTest::testUrldecode() {
	testUrldecodeWith<BytesType>();
	testUrldecodeWith<std::string>();
}

void 
UrlEncodeTest::testUrldecodeEmpty() {
	testUrldecodeEmptyWith<BytesType>();
	testUrldecodeEmptyWith<std::string>();
}

void
UrlEncodeTest::testUrldecodeLatin() {
	testUrldecodeLatinWith<BytesType>();
	testUrldecodeLatinWith<std::string>();
}

void
UrlEncodeTest::testUrldecodeBadSuffix() {
	testUrldecodeBadSuffixWith<BytesType>();
	testUrldecodeBadSuffixWith<std::string>();
}

template <typename Cont> inline void
UrlEncodeTest::testUrlencodeWith() {
	using namespace nextweb::details;
	Cont cont = as<Cont>("раз два три четыре пять"), res = urlencode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string("%D2%C1%DA%20%C4%D7%C1%20%D4%D2%C9%20%DE%C5%D4%D9%D2%C5%20%D0%D1%D4%D8"), makeString(res));
}

template <typename Cont> inline void
UrlEncodeTest::testUrlencodeEmptyWith() {
	using namespace nextweb::details;
	Cont cont, res = urlencode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string(""), makeString(res));
}

template <typename Cont> inline void
UrlEncodeTest::testUrlencodeLatinWith() {
	using namespace nextweb::details;
	Cont cont = as<Cont>("abcd efgh"), res = urlencode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string("abcd%20efgh"), makeString(res));
}

template <typename Cont> inline void
UrlEncodeTest::testUrldecodeWith() {
	using namespace nextweb::details;
	Cont cont = as<Cont>("%D2%C1%DA%20%C4%D7%C1%20%D4%D2%C9%20%DE%C5%D4%D9%D2%C5%20%D0%D1%D4%D8"), res = urldecode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string("раз два три четыре пять"), makeString(res));
}

template <typename Cont> inline void
UrlEncodeTest::testUrldecodeEmptyWith() {
	using namespace nextweb::details;
	Cont cont, res = urldecode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string(""), makeString(res));
}

template <typename Cont> inline void
UrlEncodeTest::testUrldecodeLatinWith() {
	using namespace nextweb::details;
	Cont cont = as<Cont>("abcd%20efgh"), res = urldecode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string("abcd efgh"), makeString(res));
}

template <typename Cont> inline void
UrlEncodeTest::testUrldecodeBadSuffixWith() {
	using namespace nextweb::details;
	Cont cont = as<Cont>("abcd%20efgh%"), res = urldecode(cont);
	CPPUNIT_ASSERT_EQUAL(std::string("abcd efgh%"), makeString(res));
}

}} // namespaces
