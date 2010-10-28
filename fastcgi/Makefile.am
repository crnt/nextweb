lib_LTLIBRARIES = libnextweb-fastcgi.la

libnextweb_fastcgi_la_LIBADD = ${top_srcdir}/utils/libnextweb.la
libnextweb_fastcgi_la_SOURCES = \
	Cookie.cpp \
	FCGI.cpp \
	File.cpp \
	FileParser.cpp \
	HttpDate.cpp \
	HttpError.cpp \
	HttpUtils.cpp \
	Logger.cpp \
	Request.cpp \
	RequestHandler.cpp \
	Response.cpp \
	Server.cpp \
	ServerImpl.cpp \
	Settings.cpp \
	UrlMatch.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config
AM_LDFLAGS = @FCGI_LIBS@
