lib_LTLIBRARIES = libnextweb-fastcgi.la

libnextweb_fastcgi_la_LIBADD = ${top_srcdir}/utils/libnextweb.la
libnextweb_fastcgi_la_SOURCES = \
	Buffer.cpp \
	Cookie.cpp \
	FastCgiIO.cpp \
	File.cpp \
	FileImpl.cpp \
	GenericResponse.cpp \
	HttpDate.cpp \
	HttpError.cpp \
	HttpStatus.cpp \
	HttpUtils.cpp \
	IterFileImpl.cpp \
	Logger.cpp \
	PostParser.cpp \
	Request.cpp \
	RequestHandler.cpp \
	Response.cpp \
	Server.cpp \
	ServerImpl.cpp \
	Settings.cpp \
	SocketPolicy.cpp \
	UrlMatch.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config
AM_LDFLAGS = @FCGI_LIBS@
