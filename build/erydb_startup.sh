#!/bin/bash
#
# $Id: erydb_startup.sh 3705 2013-08-07 19:47:20Z dhill $
#
# erydb_startup.sh steps for erydb single server startup after install

prefix=/usr/local
installdir=$prefix/erydb

quiet=0

for arg in "$@"; do
	if [ `expr -- "$arg" : '--prefix='` -eq 9 ]; then
		prefix="`echo $arg | awk -F= '{print $2}'`"
		installdir=$prefix/erydb
	elif [ `expr -- "$arg" : '--installdir='` -eq 13 ]; then
		installdir="`echo $arg | awk -F= '{print $2}'`"
		prefix=`dirname $installdir`
	else
		echo "erydb_startup.sh: ignoring unknown argument: $arg" 1>&2
	fi
done

$installdir/bin/post-install --installdir=/usr/local/erydb
echo -e "1\n1\n1\n1\n1\n1\n" | $installdir/bin/postConfigure -i $installdir
if [ $? -eq 0 ]; then
	exit 0
else
	exit 1
fi
