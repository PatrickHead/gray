#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gray],[0.1.0],[pth@patrickhead.net])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])
LT_INIT

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([ ctype.h errno.h getopt.h libgen.h stdio.h stdlib.h string.h sys/stat.h sys/types.h time.h unistd.h ])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_MODE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset mkdir strchr strdup strtol])

# Checks for required packages
PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.9)

AC_CONFIG_FILES([
  Makefile
  tools/Makefile
  include/Makefile
  src/Makefile
  tests/Makefile
])

AC_OUTPUT
