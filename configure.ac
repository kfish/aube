dnl Process this file with autoconf to produce a configure script.
AC_INIT()

AM_INIT_AUTOMAKE(aube, 0.30.2)

dnl Specify a header configuration file
AM_CONFIG_HEADER(config.h)

dnl Initialize libtool
AM_PROG_LIBTOOL

dnl Initialize maintainer mode
AM_MAINTAINER_MODE


dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CPP
AC_PROG_MAKE_SET

dnl Checks for libraries
PKG_CHECK_MODULES(SNDFILE, sndfile >= 1.0.0,
                  HAVE_LIBSNDFILE="yes", HAVE_LIBSNDFILE="no")

if test "x$HAVE_LIBSNDFILE" = xyes ; then
  AC_DEFINE([HAVE_LIBSNDFILE], [], [Define if we have libsndfile.])
  AC_SUBST(SNDFILE_LIBS)
  fi

PKG_CHECK_MODULES(GTK, gtk+-2.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)


AM_PATH_GDK_IMLIB(1.9, ,AC_MSG_ERROR([*** IMLIB 1.9 not installed ***]))
dnl Test for libXpm
AC_CHECK_LIB(Xpm, XpmReadFileToXpmImage)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h strings.h sys/ioctl.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_PROG_GCC_TRADITIONAL

AC_ARG_ENABLE(gcc-werror,
	AC_HELP_STRING([--enable-gcc-werror], [enable -Werror in all Makefiles]))

dnl Use -Wall if we have gcc.
if test "x$ac_cv_prog_gcc" = xyes ; then
  CFLAGS="$CFLAGS -Wall"

  if test x$enable_gcc_werror = "xyes" ; then
    CFLAGS="$CFLAGS -Werror"
	fi

fi


AC_OUTPUT(
Makefile
src/Makefile
src/modules/Makefile
)
