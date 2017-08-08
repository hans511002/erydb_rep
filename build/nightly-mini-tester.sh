#!/bin/bash

NO_NONROOT_SUDO=yes
export NO_NONROOT_SUDO
prefix=/usr/local
builddir=
for arg in "$@"; do
	if [ `expr -- "$arg" : '--prefix='` -eq 9 ]; then
		prefix="`echo $arg | awk -F= '{print $2}'`"
	elif [ `expr -- "$arg" : '--builddir='` -eq 11 ]; then
		builddir="`echo $arg | awk -F= '{print $2}'`"
	else
		echo "ignoring unknown argument: $arg" 1>&2
	fi
done

if [ -z "$builddir" ]; then
	echo "I really need a builddir to continue!" 1>&2
	exit 1
fi

if [ ! -d ${builddir}/export/erydb ]; then
	echo "I did't find a erydb dir in ${builddir}/export!" 1>&2
	exit 1
fi

# stop any current procs
${prefix}/erydb/bin/eryadmin shutdownsystem y
if [ -x ${prefix}/erydb/bin/erydb ]; then
	${prefix}/erydb/bin/erydb stop
fi

# really stop current procs
sleep 5
for proc in DMLProc DDLProc ExeMgr PrimProc controllernode workernode; do
	pkill -9 $proc
	sleep 1
done
if [ -x ${prefix}/erydb/mysql/mysql-erydb ]; then
	${prefix}/erydb/mysql/mysql-erydb stop
fi

# cleanup

#    remove shm segs
if [ -x ${prefix}/erydb/bin/clearShm ]; then
	${prefix}/erydb/bin/clearShm stop
fi
#    remove erydb dir
/usr/local/bin/rootStuff
rm -rf ${prefix}/erydb 2>/dev/null
/usr/local/bin/rootStuff

#    (we'll leave the logging stuff in place for now)

# install the binaries
tar -C ${builddir}/export -cf - erydb | tar -C ${prefix} -xf -
if [ $? -ne 0 ]; then
	echo "There was a problem installing the binaries!" 1>&2
	exit 1
fi
#chown -R root.root ${prefix}/erydb
find ${prefix}/erydb -type d | xargs chmod +rx
find ${prefix}/erydb -type f | xargs chmod +r

mkdir -p ${prefix}/erydb/data1/systemFiles/dbrm

if [ ! -f ${prefix}/erydb/etc/erydb.xml.rpmsave ]; then
	cp ${prefix}/erydb/etc/erydb.xml.singleserver ${prefix}/erydb/etc/erydb.xml.rpmsave
fi

if [ ! -f ${prefix}/erydb/mysql/my.cnf ]; then
	cp ${builddir}/dbcon/mysql/my.cnf ${prefix}/erydb/mysql
fi

#fix the port numbers
sed -i -e 's/port.*=.*3306/port=14406/' ${prefix}/erydb/mysql/my.cnf

# configure the s/w
${prefix}/erydb/bin/postConfigure -n

# restart (argh)
#${prefix}/erydb/bin/eryadmin RestartSystem y

sleep 30
pkill DMLProc
sleep 30

# perform the tests
if [ ! -x ${builddir}/build/mini-tests.sh ]; then
	echo "There was a problem trying to start testing the s/w!" 1>&2
	exit 1
fi
${builddir}/build/mini-tests.sh --prefix=${prefix}
if [ $? -ne 0 ]; then
	echo "There were problems running the tests!" 1>&2
	exit 1
fi

# stop the system
${prefix}/erydb/bin/erydb stop
${prefix}/erydb/mysql/mysql-erydb stop

exit 0

