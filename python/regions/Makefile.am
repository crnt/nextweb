pkgpyexec_LTLIBRARIES = regions.la

regions_la_LIBADD = ${top_srcdir}/regions/libnextweb-regions.la
regions_la_LDFLAGS = -module
regions_la_SOURCES = \
	module.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config @PYTHON_CPPFLAGS@
AM_LDFLAGS = @PYTHON_LDFLAGS@

