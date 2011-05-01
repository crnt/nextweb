AC_DEFUN([AX_CHECK_CPPUNIT],
[
	AC_REQUIRE([AM_PATH_CPPUNIT])
	
	ax_have_cppunit="no"
	ax_cppunit_found="no"
	AC_ARG_ENABLE(cppunit, 
		AS_HELP_STRING(--enable-cppunit,enables cppunit tests compilation),
	[
		if test "f$enableval" = "fyes"; then
			ax_have_cppunit="yes"
		fi
	])

	if test "f$ax_have_cppunit" = "fyes"; then
		ax_cppunit_found="yes"
		
		AC_LANG_SAVE
		AC_LANG_CPLUSPLUS
		AM_PATH_CPPUNIT([$1], [], [ax_cppunit_found="no"])
	
		ax_check_cppunit_stored_libs="$LIBS"
		ax_check_cppunit_stored_cppflags="$CPPFLAGS"
		LIBS="$ax_check_cppunit_stored_libs $CPPUNIT_LIBS"
		CPPFLAGS="$ax_check_cppunit_stored_cppflags $CPPUNIT_CFLAGS"

		AC_MSG_CHECKING([linkage with cppunit])
		AC_LINK_IFELSE(
			[AC_LANG_PROGRAM([#include <cppunit/ui/text/TestRunner.h>], [CppUnit::TextUi::TestRunner r;])],
			[AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); ax_cppunit_found="no"])
	
		CPPFLAGS="$ax_check_cppunit_stored_cppflags"
		LIBS="$ax_check_cppunit_stored_libs"
		
		AC_LANG_RESTORE
		if test "f$ax_cppunit_found" = "fyes"; then
			AC_DEFINE([HAVE_CPPUNIT], 1, [Define to 1 if you want to compile cppunit tests])
			ifelse([$2], , :, [$2])
		else
			ifelse([$3], , :, [$3])
		fi	
	fi
	AM_CONDITIONAL(HAVE_CPPUNIT, [test "f$ax_cppunit_found" = "fyes"])
])
