lib_LTLIBRARIES = libnextweb-templates.la

libnextweb_templates_la_LIBADD = ${top_srcdir}/utils/libnextweb.la
libnextweb_templates_la_SOURCES = \
	ParamSet.cpp \
	Template.cpp \
	TemplateImpl.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config
AM_LDFLAGS = 
