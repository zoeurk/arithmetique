#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([zoeurk@gmail.com], [1.0], [zoeurk@gmail.com])
AC_CONFIG_SRCDIR([arithmetique/arithmetique.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-larithmetique':
AC_CHECK_LIB([arithmetique], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-loperation':
AC_CHECK_LIB([operation], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])
LT_INIT
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_MACRO_DIRS([m4])
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strchr])

AC_CONFIG_FILES([Makefile
		operation/Makefile
		arithmetique/Makefile])
AC_OUTPUT
