# Copyright (C) 2014 EryDB, Inc.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2 of
# the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

# $Id: Makefile.am 333 2009-04-03 20:35:04Z rdempsey $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = $(erydb_common_includes) $(erydb_cppflags)
AM_CFLAGS = $(erydb_cflags)
AM_CXXFLAGS = $(erydb_cxxflags)
AM_LDFLAGS = $(erydb_ldflags)
bin_PROGRAMS = PrimProc
PrimProc_SOURCES = primproc.cpp \
        batchprimitiveprocessor.cpp \
        bppseeder.cpp \
        bppsendthread.cpp \
        columncommand.cpp \
        command.cpp \
        dictstep.cpp \
        filtercommand.cpp \
        logger.cpp \
        passthrucommand.cpp \
        primitiveserver.cpp \
	pseudocc.cpp \
        rtscommand.cpp \
        umsocketselector.cpp
PrimProc_CPPFLAGS = -I../blockcache -I../linux-port $(AM_CPPFLAGS)
PrimProc_CXXFLAGS = $(march_flags) $(AM_CXXFLAGS)
PrimProc_LDFLAGS = $(erydb_common_ldflags) $(erydb_write_libs) $(erydb_common_libs) -lthreadpool -lcacheutils $(netsnmp_libs) $(AM_LDFLAGS)
PrimProc_LDADD = ../blockcache/libdbbc.a ../linux-port/libprocessor.a

test:

coverage:

leakcheck:

docs:

bootstrap: install-data-am

