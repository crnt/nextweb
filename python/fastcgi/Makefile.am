pkgpyexec_LTLIBRARIES = fastcgi.la

fastcgi_la_LIBADD = ${top_srcdir}/fastcgi/libnextweb-fastcgi.la
fastcgi_la_LDFLAGS = -module
fastcgi_la_SOURCES = module.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config @PYTHON_CPPFLAGS@
AM_LDFLAGS = @PYTHON_LDFLAGS@
