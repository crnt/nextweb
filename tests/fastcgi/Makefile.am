check_PROGRAMS = fastcgi-tests

fastcgi_tests_LDADD = ${top_srcdir}/fastcgi/libnextweb-fastcgi.la \
	${top_srcdir}/utils/libnextweb.la
	
fastcgi_tests_SOURCES = main.cpp MockIO.cpp \
	CookieTest.cpp \
	HttpDateTest.cpp \
	LineEndTest.cpp \
	RequestTest.cpp \
	StreamBufferTest.cpp \
	UrlEncodeTest.cpp \
	UrlMatchTest.cpp \
	VarNameTest.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	-I${top_srcdir}/tests @CPPUNIT_CFLAGS@
AM_LDFLAGS = @CPPUNIT_LIBS@

TESTS = fastcgi-tests

noinst_HEADERS = MockIO.hpp
noinst_DATA = data/copying.tst data/install.tst data/multipart.tst
