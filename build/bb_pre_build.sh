#!/bin/bash

prefix="$1"

if [ -z "$prefix" ]; then
	echo "usage: $0 destdir" 1>&2
	exit 1
fi

rm -rf erydb mysql erydb-ent

git clone http://srvgip1.erydb.com/repos/erydb.git
cd erydb
git checkout develop
cd ..

git clone https://github.com/erydb/mysql.git
cd mysql
git checkout develop
cd ..

git clone http://srvgip1.erydb.com/repos/erydb-ent.git
cd erydb-ent
git checkout develop
cd ..

cd erydb

mkdir -p export/{include,lib,etc,share,bin,sbin,post}

cp -r utils/autoconf/* .
autoreconf
libtoolize --force --install

./configure --prefix=$prefix

