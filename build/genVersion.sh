#!/bin/bash
#
# $Id: genVersion.sh 1019 2009-12-18 14:42:35Z rdempsey $
#

#prefix=export
#for arg in "$@"; do
#	if [ `expr -- "$arg" : '--prefix='` -eq 9 ]; then
#		prefix="`echo $arg | awk -F= '{print $2}'`"
#	else
#		echo "ignoring unknown argument: $arg" 1>&2
#	fi
#done
#
##try to find project root
#while [ ! -d dbcon ]; do
#	cd ..
#	curdir=$(pwd)
#	if [ $curdir = / -o $curdir = $HOME ]; then
#		echo "I could not find the project root directory: I can't continue!"
#		exit 1
#	fi
#done
#
#if [ ! -f ./build/releasenum ]; then
#	echo "I could not find the file 'releasesum' in the build directory: I can't continue!"
#	exit 1
#fi

. ./build/releasenum

#mkdir -p ${prefix}/include

echo "
#ifndef VERSIONNUMBER_H_
#define VERSIONNUMBER_H_
#include <string>
const std::string erydb_version(\"$version\");
const std::string erydb_release(\"$release\");
#endif
" > dbcon/mysql/versionnumber.h.tmp

diff -bBq dbcon/mysql/versionnumber.h.tmp dbcon/mysql/versionnumber.h >/dev/null 2>&1
if [ $? -ne 0 ]; then
	cp dbcon/mysql/versionnumber.h.tmp dbcon/mysql/versionnumber.h
fi

rm -f dbcon/mysql/versionnumber.h.tmp

