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

# $Id: Makefile.am 3048 2012-04-04 15:33:45Z rdempsey $
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = $(erydb_common_includes) $(erydb_cppflags)
AM_CFLAGS = $(erydb_cflags)
AM_CXXFLAGS = $(erydb_cxxflags)
AM_LDFLAGS = -version-info 1:0:0 $(erydb_ldflags)
lib_LTLIBRARIES = libloggingcpp.la
libloggingcpp_la_SOURCES = message.cpp messagelog.cpp logger.cpp errorcodes.cpp sqllogger.cpp stopwatch.cpp erydberrorinfo.cpp
include_HEADERS = loggingid.h messageobj.h messagelog.h messageids.h logger.h errorcodes.h exceptclasses.h sqllogger.h stopwatch.h erydberrorinfo.h errorids.h
dist_sysconf_DATA = MessageFile.txt ErrorMessage.txt
BUILT_SOURCES = messageids.h errorids.h

test:

coverage:

leakcheck:

docs:

bootstrap: install-data-am

messageids.h: MessageFile.txt genMsgId.pl
	./genMsgId.pl > messageids-temp.h
	diff -abBq messageids-temp.h messageids.h >/dev/null 2>&1; \
	if [ $$? -ne 0 ]; then \
		mv -f messageids-temp.h messageids.h; \
	else \
		touch messageids.h; \
	fi; \
	rm -f messageids-temp.h

errorids.h: ErrorMessage.txt genErrId.pl
	./genErrId.pl > errorids-temp.h
	diff -abBq errorids-temp.h errorids.h >/dev/null 2>&1; \
	if [ $$? -ne 0 ]; then \
		mv -f errorids-temp.h errorids.h; \
	else \
		touch errorids.h; \
	fi; \
	rm -f errorids-temp.h

