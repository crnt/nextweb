noinst_LTLIBRARIES = libnextweb.la

libnextweb_la_SOURCES = Condition.cpp EnvironIterator.cpp Error.cpp \
	HttpUtils.cpp MmapImpl.cpp Mutex.cpp RuntimeSettings.cpp ScopedLock.cpp \
	SystemError.cpp Thread.cpp ThreadGroup.cpp

libnextweb_la_LDFLAGS = -disable-shared

lib_LTLIBRARIES = libnextweb-daemon.la

libnextweb_daemon_la_SOURCES = Environment.cpp Request.cpp RequestImpl.cpp \
	Response.cpp Server.cpp

libnextweb_daemon_la_LDFLAGS = -lfcgi
libnextweb_daemon_la_LIBADD = libnextweb.la

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I../include -I../config
