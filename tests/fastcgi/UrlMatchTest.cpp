#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TestUtils.hpp"
#include "nextweb/utils/Range.hpp"
#include "nextweb/fastcgi/impl/UrlMatch.hpp"

namespace nextweb { namespace tests {

class UrlMatchTest : public CppUnit::TestFixture {

public:
	void testMatch();
	void testNotMatch();
	void testMatchCompare();

private:
	template <typename Sequence> void testMatchWith();
	template <typename Sequence> void testNotMatchWith();
	template <typename Sequence> void testMatchCompareWith();

	typedef std::list<char> CharList;
	typedef std::vector<char> CharVector;
	typedef utils::Range<char const*> CharSequence;

private:
	CPPUNIT_TEST_SUITE(UrlMatchTest);
	CPPUNIT_TEST(testMatch);
	CPPUNIT_TEST(testNotMatch);
	CPPUNIT_TEST(testMatchCompare);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(UrlMatchTest);

void
UrlMatchTest::testMatch() {
	testMatchWith<CharList>();
	testMatchWith<CharVector>();
	testMatchWith<std::string>();
	testMatchWith<CharSequence>();
}

void
UrlMatchTest::testNotMatch() {
	testNotMatchWith<CharList>();
	testNotMatchWith<CharVector>();
	testNotMatchWith<std::string>();
	testNotMatchWith<CharSequence>();
}

void
UrlMatchTest::testMatchCompare() {
	testMatchCompareWith<CharList>();
	testMatchCompareWith<CharVector>();
	testMatchCompareWith<std::string>();
	testMatchCompareWith<CharSequence>();
}

template <typename Sequence> void
UrlMatchTest::testMatchWith() {

	using namespace fastcgi;
	std::pair<bool, std::size_t> res;
	
	res = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/*/test"));
	CPPUNIT_ASSERT_EQUAL(true, res.first);
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(2), res.second);
	
	res = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/misc/test"));
	CPPUNIT_ASSERT_EQUAL(true, res.first);
	CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(0), res.second);
}

template <typename Sequence> void
UrlMatchTest::testNotMatchWith() {

	using namespace fastcgi;
	std::pair<bool, std::size_t> res;
	
	res = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/other/*"));
	CPPUNIT_ASSERT_EQUAL(false, res.first);

	res = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/*/other"));
	CPPUNIT_ASSERT_EQUAL(false, res.first);
	
	res = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/*/other/other"));
	CPPUNIT_ASSERT_EQUAL(false, res.first);
}

template <typename Sequence> void
UrlMatchTest::testMatchCompareWith() {
	
	using namespace fastcgi;

	std::pair<bool, std::size_t> r1 = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/misc/test"));
	std::pair<bool, std::size_t> r2 = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/misc/*"));
	std::pair<bool, std::size_t> r3 = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/nextweb/*/test"));
	std::pair<bool, std::size_t> r4 = urlMatch(as<Sequence>("/nextweb/misc/test"), as<Sequence>("/*/misc/test"));
	
	CPPUNIT_ASSERT_EQUAL(true, r4.second > r3.second);
	CPPUNIT_ASSERT_EQUAL(true, r3.second > r2.second);
	CPPUNIT_ASSERT_EQUAL(true, r2.second > r1.second);
}

}} // namespaces
