#!/bin/bash

erydbBranch=$1
mysqlBranch=$2

if [ -z "$erydbBranch" ]; then
        echo Usage: $0 erydb-branch mysql-branch 1>&2
        exit 1
fi

if [ "$erydbBranch" != "current" ]; then
	if [ -z "$mysqlBranch" ]; then
	        echo Usage: $0 erydb-branch mysql-branch  1>&2
        	exit 1
	fi
else
	mysqlBranch=$1
fi

{
if [ "$erydbBranch" != "current" ]; then
	rm -rf EryDB-MySQL
	rm -rf EryDB
	rm -rf EryDB-Enterprise

	git clone https://github.com/mariadb-corporation/EryDB
	git clone https://github.com/mariadb-corporation/EryDB-MySQL
	git clone https://github.com/mariadb-corporation/EryDB-Enterprise
fi

echo "****** Compile EryDB-MySQL"
cd EryDB-MySQL
if [ "$erydbBranch" != "current" ]; then
	git checkout $mysqlBranch
fi
./build-MySQL $mysqlBranch

echo "****** Compile EryDB"
cd ../EryDB
if [ "$erydbBranch" != "current" ]; then
	git checkout $erydbBranch
fi
./build/bootstrap
make -j4
make -j4
make install

echo "****** Compile EryDB-Enterprise"
cd ../EryDB-Enterprise
if [ "$erydbBranch" != "current" ]; then
	git checkout $erydbBranch
fi
make -j4
make -j4
make install

echo "****** Generate RPMS"
cd ../EryDB
./build/build_rpms
} > erydb-build-$erydbBranch.log 2>&1
 
echo "EryDB Build Completed for $erydbBranch"
exit 0

