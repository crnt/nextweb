check_PROGRAMS = nextweb-tests

nextweb_tests_LDADD = ../library/libnextweb-daemon.la
nextweb_tests_SOURCES = main.cpp ConvertersTest.cpp EnumerationTest.cpp \
	EnvironIteratorTest.cpp FileTest.cpp HttpDateTest.cpp IteratorTest.cpp \
	LinkageTest.cpp RangeTest.cpp RequestTest.cpp SharedTest.cpp \
	SplitTest.cpp StartsTest.cpp ThreadTest.cpp TrimTest.cpp \
	UrlEncodeTest.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I../include -I../config @CPPUNIT_CFLAGS@
AM_LDFLAGS = @CPPUNIT_LIBS@

noinst_HEADERS = MockEnvironment.hpp MockIterator.hpp TestUtils.hpp

TESTS = nextweb-tests

CLEANFILES = write*.tmp

EXTRA_DIST = data/get data/post
