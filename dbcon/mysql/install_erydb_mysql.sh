#!/bin/bash
#
# $Id$
#

prefix=/usr/local
installdir=$prefix/erydb
rpmmode=install
pwprompt=" "

for arg in "$@"; do
	if [ `expr -- "$arg" : '--prefix='` -eq 9 ]; then
		prefix="`echo $arg | awk -F= '{print $2}'`"
		installdir=$prefix/erydb
	elif [ `expr -- "$arg" : '--rpmmode='` -eq 10 ]; then
		rpmmode="`echo $arg | awk -F= '{print $2}'`"
	elif [ `expr -- "$arg" : '--password='` -eq 11 ]; then
		password="`echo $arg | awk -F= '{print $2}'`"
		pwprompt="--password=$password"
	elif [ `expr -- "$arg" : '--installdir='` -eq 13 ]; then
		installdir="`echo $arg | awk -F= '{print $2}'`"
		prefix=`dirname $installdir`
	else
		echo "ignoring unknown argument: $arg" 1>&2
	fi
done

df=$installdir/mysql/my.cnf

$installdir/mysql/bin/mysql --defaults-file=$df --force --user=root $pwprompt mysql 2>/tmp/mysql_install.log <<EOD
INSTALL PLUGIN erydb SONAME 'liberymysql.so';
INSTALL PLUGIN erydb SONAME 'liberymysql.so';
INSTALL PLUGIN erydb_tables SONAME 'is_erydb_tables.so';
INSTALL PLUGIN erydb_columns SONAME 'is_erydb_columns.so';
INSTALL PLUGIN erydb_extents SONAME 'is_erydb_extents.so';
INSTALL PLUGIN erydb_files SONAME 'is_erydb_files.so';
-- these are deprecated names
DELETE FROM mysql.func WHERE name='erydisablepartition';
DELETE FROM mysql.func WHERE name='erydroppartition';
DELETE FROM mysql.func WHERE name='eryenablepartition';
DELETE FROM mysql.func WHERE name='erydisablepartitions';
DELETE FROM mysql.func WHERE name='eryenablepartitions';
DELETE FROM mysql.func WHERE name='erydroppartitions';
DELETE FROM mysql.func WHERE name='eryshowpartitions';
DELETE FROM mysql.func WHERE name='erydroppartitionsbyvalue';
DELETE FROM mysql.func WHERE name='erydisablepartitionsbyvalue';
DELETE FROM mysql.func WHERE name='eryenablepartitionsbyvalue';
DELETE FROM mysql.func WHERE name='eryshowpartitionsbyvalue';
-- these functions moved libs
DROP FUNCTION erydisablepartitions;
DROP FUNCTION eryenablepartitions;
DROP FUNCTION erydroppartitions;
DROP FUNCTION eryshowpartitions;
DROP FUNCTION erydroppartitionsbyvalue;
DROP FUNCTION erydisablepartitionsbyvalue;
DROP FUNCTION eryenablepartitionsbyvalue;
DROP FUNCTION eryshowpartitionsbyvalue;
CREATE FUNCTION erygetstats RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erysettrace RETURNS INTEGER SONAME 'liberymysql.so';
CREATE FUNCTION erysetparms RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION eryflushcache RETURNS INTEGER SONAME 'liberymysql.so';
CREATE FUNCTION erygettrace RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erygetversion RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION eryonlinealter RETURNS INTEGER SONAME 'liberymysql.so';
CREATE FUNCTION eryviewtablelock RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erycleartablelock RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erydisablepartitions RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION eryenablepartitions RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erydroppartitions RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION eryshowpartitions RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erylastinsertid RETURNS INTEGER SONAME 'liberymysql.so';
CREATE FUNCTION erydroppartitionsbyvalue RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erydisablepartitionsbyvalue RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION eryenablepartitionsbyvalue RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION eryshowpartitionsbyvalue RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erygetsqlcount RETURNS STRING SONAME 'liberymysql.so';
CREATE FUNCTION erydbpm RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbdbroot RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbsegment RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbsegmentdir RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbextentrelativerid RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbblockid RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbextentid RETURNS INTEGER soname 'liberymysql.so';
CREATE FUNCTION erydbextentmin RETURNS STRING soname 'liberymysql.so';
CREATE FUNCTION erydbextentmax RETURNS STRING soname 'liberymysql.so';
CREATE FUNCTION erydbpartition RETURNS STRING soname 'liberymysql.so';
CREATE FUNCTION erydblocalpm RETURNS INTEGER soname 'liberymysql.so';

CREATE DATABASE IF NOT EXISTS erydb_vtable;
CREATE DATABASE IF NOT EXISTS erydb_querystats;
CREATE TABLE IF NOT EXISTS erydb_querystats.querystats
(
  queryID bigint NOT NULL AUTO_INCREMENT,
  sessionID bigint DEFAULT NULL,
  host varchar(50),
  user varchar(50),
  priority char(20),
  queryType char(25),
  query varchar(8000),
  startTime timestamp NOT NULL,
  endTime timestamp NOT NULL,
  rows bigint,
  errno int,
  phyIO bigint,
  cacheIO bigint,
  blocksTouched bigint,
  CPBlocksSkipped bigint,
  msgInUM bigint,
  msgOutUm bigint,
  maxMemPct int,
  blocksChanged bigint,
  numTempFiles bigint,
  tempFileSpace bigint,
  PRIMARY KEY (queryID)
);

CREATE TABLE IF NOT EXISTS erydb_querystats.user_priority
(
  host varchar(50),
  user varchar(50),
  priority char(20)
) DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS erydb_querystats.priority
(
  priority char(20) primary key,
  priority_level int
) DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

insert ignore into erydb_querystats.priority values ('High', 100),('Medium', 66), ('Low', 33);
grant all privileges on  *.* to 'root'@'%' with grant option; 
create user 'mydb' identified by 'mydb';  
grant all privileges on  *.* to 'mydb'@'%' with grant option; 
create database mydb  character set utf8;
flush privileges;  
EOD

$installdir/mysql/bin/mysql --defaults-file=$df --user=root $pwprompt mysql 2>/dev/null <$installdir/mysql/syscatalog_mysql.sql
$installdir/mysql/bin/mysql --defaults-file=$df --user=root $pwprompt mysql 2>/dev/null <$installdir/mysql/erysetuserpriority.sql
$installdir/mysql/bin/mysql --defaults-file=$df --user=root $pwprompt mysql 2>/dev/null <$installdir/mysql/eryremoveuserpriority.sql
$installdir/mysql/bin/mysql --defaults-file=$df --user=root $pwprompt mysql 2>/dev/null <$installdir/mysql/eryshowprocesslist.sql
$installdir/mysql/bin/mysql --defaults-file=$df --user=root $pwprompt mysql 2>/dev/null <$installdir/mysql/erydb_info.sql

sed -i 's/erydb_compression_type=1/erydb_compression_type=2/' $installdir/mysql/my.cnf >/dev/null 2>&1

