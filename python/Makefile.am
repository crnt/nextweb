pkgpyexec_LTLIBRARIES = server.la templates.la auth.la regions.la

server_la_SOURCES = Server.cpp
server_la_LDFLAGS = -module -avoid-version
server_la_LIBADD = ../library/libnextweb.la

templates_la_SOURCES = Templates.cpp
templates_la_LDFLAGS = -module -avoid-version
templates_la_LIBADD = ../library/libnextweb.la

auth_la_SOURCES = Auth.cpp
auth_la_LDFLAGS = -module -avoid-version
auth_la_LIBADD = ../library/libnextweb.la

regions_la_SOURCES = Regions.cpp
regions_la_LDFLAGS = -module -avoid-version
regions_la_LIBADD = ../library/libnextweb.la

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I../config -I../include @PYTHON_CPPFLAGS@
AM_LDFLAGS = @PYTHON_LDFLAGS@

bin_SCRIPTS = nextweb-regc nextweb-compile

EXTRA_DIST = nextweb-compile.in nextweb-regc.in 
