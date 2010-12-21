check_PROGRAMS = utils-tests

utils_tests_LDADD = ${top_srcdir}/utils/libnextweb.la
utils_tests_SOURCES = main.cpp MockShared.cpp MockThread.cpp \
	ConvertersTest.cpp \
	EnumerationTest.cpp \
	FunctorsTest.cpp \
	IntegerTest.cpp \
	IteratorTest.cpp \
	RangeTest.cpp \
	SharedPtrTest.cpp \
	StaticAssertTest.cpp \
	StringTest.cpp \
	ThreadsTest.cpp \
	TokenizerTest.cpp \
	TupleTest.cpp \
	TypeListTest.cpp \
	TypeTraitsTest.cpp \
	VarUtilsTest.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	-I${top_srcdir}/tests @CPPUNIT_CFLAGS@
AM_LDFLAGS = @CPPUNIT_LIBS@

noinst_HEADERS = MockIterator.hpp MockShared.hpp MockTemplate.hpp \
	MockThread.hpp

TESTS = utils-tests

CLEANFILES = *.dat
