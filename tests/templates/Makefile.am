check_PROGRAMS = templates-tests

templates_tests_LDADD = ${top_srcdir}/templates/libnextweb-templates.la \
	${top_srcdir}/utils/libnextweb.la
	
templates_tests_SOURCES = main.cpp ParamsTest.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	-I${top_srcdir}/tests @CPPUNIT_CFLAGS@
AM_LDFLAGS = @CPPUNIT_LIBS@

TESTS = templates-tests
