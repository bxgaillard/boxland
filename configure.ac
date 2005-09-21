# Process this file with autoconf to produce a configure script.

# General settings
AC_PREREQ(2.59)
AC_INIT([boxland], [1.1.0], [core@powercode.net])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs
AC_LANG([C++])
AC_PROG_CXX

# Check for pkg-config
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = "xno"; then
    AC_MSG_ERROR([pkg-config is mandatory to configure gtkmm.])
fi

# Checks for header files
AC_HEADER_DIRENT
AC_HEADER_STAT

# Checks for library functions
AC_HEADER_STDC
AC_CHECK_FUNCS([memset])

# Check for gtkmm
PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4])
AC_SUBST([GTKMM_CFLAGS])
AC_SUBST([GTKMM_LIBS])

# Enable G++ warnings
if test "x$GXX" = "xyes"; then
    CXXFLAGS="-Wall -W $CXXFLAGS"
fi

# Generated files
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    images/Makefile
    levels/Makefile
])
AC_OUTPUT
