# Copyright (C) 2008-2010, 2012 D. V. Wiebe
#
##########################################################################
#
# This file is part of the GetData project.
#
# GetData is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation; either version 2.1 of the License, or (at your
# option) any later version.
#
# GetData is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with GetData; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
AUTOMAKE_OPTIONS = foreign

if FC_WALL
FC_WALL=-Wall
endif

if F77_WALL
F77_WALL=-Wall
endif

if F77_FNO_BACKSLASH
F77_FNO_BACKSLASH=-fno-backslash
endif

if INTEL_F77_COMPILER
F77_ASSUME=-assume byterecl
endif

if INTEL_FC_COMPILER
FC_ASSUME=-assume byterecl
endif

if MAKE_F95BINDINGS
F95TESTS=big_test95
endif

BUILT_SOUCES = test_getdata.f

FFLAGS += -I. -I.. $(F77_WALL) $(F77_WEXTRA) $(F77_ASSUME) $(F77_FNO_BACKSLASH)
FCFLAGS += -I. -I.. $(F77_WALL) $(FC_WEXTRA) $(FC_ASSUME)
F95LDADD=../libf95getdata.la $(F77LDADD)
F77LDADD=../libfgetdata.la
AM_LDFLAGS = -L..

TESTS=gdcopn big_test $(F95TESTS)

gdcopn_SOURCES=gdcopn.f
gdcopn_LDADD=$(F77LDADD)

big_test_SOURCES=big_test.f
big_test_LDADD=$(F77LDADD)

big_test95_SOURCES=big_test95.f90
nodist_big_test95_SOURCES=../getdata.mod
big_test95_LDADD=$(F95LDADD)

check_PROGRAMS=$(TESTS)

clean-local:
	rm -rf test_dirfile
	rm -rf *~
