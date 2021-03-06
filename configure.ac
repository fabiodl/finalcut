#----------------------------------------------------------------------
# configure.ac  -  the Final Cut library
#----------------------------------------------------------------------

# Process this file with autoconf to produce a configure script.

AC_INIT([finalcut], [0.3.0])
AC_CONFIG_HEADER([config.h])
AX_PREFIX_CONFIG_H([src/fconfig.h], [F])
AC_CONFIG_SRCDIR([src/fobject.cpp])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS( \
		 linux/fb.h \
		 sys/io.h \
		 sys/kd.h \
		 sys/stat.h \
		 sys/time.h \
		 fcntl.h \
		 langinfo.h \
		 term.h \
		 termios.h \
		 unistd.h \
		 cmath \
		 csignal \
		 cstdlib \
                 list \
		 map \
		 queue \
		 vector)

# Checks for 'tgetent'
AC_SEARCH_LIBS([tgetent], [termcap tinfo curses ncurses])
# Checks for 'tparm'
AC_SEARCH_LIBS([tparm], [termcap tinfo curses ncurses])

# Checks for libtool
AC_ENABLE_SHARED
AC_ENABLE_STATIC
LT_INIT([dlopen])
LT_LANG([C++])
LT_OUTPUT

### This defines the version number of the installed .so files
### Update this value for every release!  (A:B:C will map to foo.so.(A-C).C.B)
### using libtool's versioning system.
AC_SUBST(SO_VERSION, ["3:0:3"])

AC_SUBST([LIBTOOL_DEPS])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile src/Makefile src/fonts/Makefile doc/Makefile test/Makefile])

# use GPM (General Purpose Mouse)
AC_ARG_WITH([gpm],
            [AS_HELP_STRING([--without-gpm], [Disable GPM mouse support])],
            [with_gpm=no],
            [with_gpm=yes])
if test "x$with_gpm" = "xyes"
then
  AC_CHECK_LIB([gpm],
               [main],
               [AC_DEFINE([HAVE_LIBGPM], 1, [Define to 1 if GPM mouse is enabled])
                LIBS="$LIBS -lgpm"])
fi

# profiling
AC_ARG_WITH([profiler],
            [AS_HELP_STRING([--with-profiler], [build extra google profiler binaries])],
 	    [with_profiler=yes],
            [with_profiler=no])
if test "x$with_profiler" = "xyes"
then
  AC_CHECK_LIB([profiler],
               [ProfilerFlush],
               [LIBS="$LIBS -lprofiler"])
fi

AC_OUTPUT
