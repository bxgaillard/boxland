#!/bin/sh
rm -rf autom4te.cache
rm -f config.h.in \
    Makefile.in src/Makefile.in images/Makefile.in levels/Makefile.in \
    aclocal.m4 configure depcomp install-sh missing \
    COPYING INSTALL
