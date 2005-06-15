# ----------------------------------------------------------------------------
#
# Boxland : un petit Boxworld
# Copyright (C) 2005 Benjamin Gaillard & Nicolas Riegel
#
# ----------------------------------------------------------------------------
#
# Fichier :     mkfiles/flags.mk
#
# Description : Paramètres de compilation
#
# ----------------------------------------------------------------------------
#
# Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le
# modifier conformément aux dispositions de la Licence Publique Générale GNU,
# telle que publiée par la Free Software Foundation ; version 2 de la
# licence, ou encore (à votre convenance) toute version ultérieure.
#
# Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE
# GARANTIE ; sans même la garantie implicite de COMMERCIALISATION ou
# D'ADAPTATION À UN OBJET PARTICULIER. Pour plus de détail, voir la Licence
# Publique Générale GNU.
#
# Vous devez avoir reçu un exemplaire de la Licence Publique Générale GNU en
# même temps que ce programme ; si ce n'est pas le cas, écrivez à la Free
# Software Foundation Inc., 675 Mass Ave, Cambridge, MA 02139, États-Unis.
#
# ----------------------------------------------------------------------------


# OS-specific Definitions
ifneq ($(OS),Windows)
    # Programs
    CC     ?= gcc
    CXX    ?= g++
    RM     ?= rm -f
    AR     ?= ar
    RANLIB ?= ranlib

    # Compiling Flags
    CFLAGS   ?= -march=$(shell uname -m) -O2 -fomit-frame-pointer -pipe
    CFLAGS   := $(CFLAGS)
    CXXFLAGS ?= $(CFLAGS)
    CXXFLAGS := $(CXXFLAGS)

    # Additional Flags
    EXESUFFIX =

    # Windows Resources
    RCFILES =
else
    # Programs
    CC     ?= gcc.exe
    CXX    ?= g++.exe
    RC     ?= windres.exe
    RM     ?= rm.exe -f
    AR     ?= ar.exe
    RANLIB ?= ranlib.exe

    # Compiling Flags
    CFLAGS   ?= -march=i386 -mtune=i686 -O2 -fomit-frame-pointer -pipe
    CXXFLAGS ?= $(CFLAGS)
    CXXFLAGS := $(CXXFLAGS)
    LDFLAGS  += -mwindows -Wl,--subsystem,windows

    # Additional Flags
    EXESUFFIX = .exe

    # Windows Resources
    RCFILES := $(wildcard *.rc)
endif

# Custom Additions to Flags
WARN      = -Wall -Wextra
CFLAGS   += $(WARN)
CXXFLAGS += -fno-check-new $(WARN)
CPPFLAGS += $(INCLUDES) -MD -MP
LDFLAGS  += -s

# End of File
