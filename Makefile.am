SUBDIRS = include utils fastcgi templates python

if HAVE_CPPUNIT
SUBDIRS += tests
endif

ACLOCAL_AMFLAGS = -I config

EXTRA_DIST = autogen.sh AUTHORS ChangeLog COPYING INSTALL NEWS README \
	config/ax_check_compiler_flags.m4 config/ax_check_cppunit.m4 \
	config/ax_check_fastcgi.m4 config/ax_prefix_config_h.m4 \
	config/ax_python_devel.m4 config/cppunit.m4 config/pkg.m4
