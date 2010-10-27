check_PROGRAMS = regions-tests

regions_tests_LDADD = ${top_srcdir}/regions/libnextweb-regions.la \
	${top_srcdir}/utils/libnextweb.la
	
regions_tests_SOURCES = main.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	-I${top_srcdir}/tests @CPPUNIT_CFLAGS@
AM_LDFLAGS = @CPPUNIT_LIBS@

TESTS = regions-tests
