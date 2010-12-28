pkgpyexec_LTLIBRARIES = templates.la

templates_la_LIBADD = ${top_srcdir}/templates/libnextweb-templates.la
templates_la_LDFLAGS = -module
templates_la_SOURCES = \
	module.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config @PYTHON_CPPFLAGS@
AM_LDFLAGS = @PYTHON_LDFLAGS@
