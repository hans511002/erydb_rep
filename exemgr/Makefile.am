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

# $Id: Makefile.am 808 2012-04-04 18:19:10Z rdempsey $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = $(erydb_common_includes) $(erydb_cppflags)
AM_CFLAGS = $(erydb_cflags)
AM_CXXFLAGS = $(erydb_cxxflags)
AM_LDFLAGS = $(erydb_ldflags)
bin_PROGRAMS = ExeMgr
ExeMgr_SOURCES = main.cpp activestatementcounter.cpp femsghandler.cpp
ExeMgr_LDFLAGS = $(erydb_common_ldflags) $(erydb_exec_libs) -lcacheutils -lthreadpool $(AM_LDFLAGS)

test:

coverage:

leakcheck:

docs:

bootstrap: install-data-am

