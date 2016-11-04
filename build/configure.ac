#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([rayscript], [0.3.0], [123hurray@gmail.com])
AC_CONFIG_SRCDIR([../src/])
AC_CONFIG_HEADER([../src/include/config.h])
AH_BOTTOM([#include <globals.h>])
AM_INIT_AUTOMAKE([foreign subdir-objects])
# Checks for programs.
AC_PROG_CC
# Checks for libraries.
AC_HEADER_STDC
AC_ARG_ENABLE([editline], AS_HELP_STRING([--disable-editline], [disable libeditline]))

AS_IF([test "x$enable_editline" != "xno"],
    [
        PKG_CHECK_MODULES(LIBEDIT, libedit)
        AC_DEFINE([HAVE_EDITLINE], [1], [Define to 1 if you want to enable libeditline])
    ]
)

AM_CONDITIONAL([EDITLINE], [test "x$enable_editline" != "xno"])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_PROG_LEX
AC_PROG_YACC

AC_ARG_ENABLE([parse-debug], AS_HELP_STRING([--enable-parse-debug], [enable debug of op code generation]))

AS_IF([test "x$enable_parse_debug" = "xyes"],
    [AC_DEFINE([PARSE_DEBUG], [1], [Define to 1 if you want to debug code generation process])]
)

AC_ARG_ENABLE([vm-debug], AS_HELP_STRING([--enable-vm-debug], [enable debug of VM execution]))
AS_IF([test "x$enable_vm_debug" = "xyes"],
    [AC_DEFINE([VM_DEBUG], [1], [Define to 1 if you want to debug VM execution])]
)


AC_CONFIG_FILES([
    Makefile
])

AC_OUTPUT

