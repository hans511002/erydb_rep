INSTALL PLUGIN erydb SONAME 'liberymysql.dll';
CREATE FUNCTION erygetstats RETURNS STRING SONAME 'liberymysql.dll';
CREATE FUNCTION erysettrace RETURNS INTEGER SONAME 'liberymysql.dll';
CREATE FUNCTION erysetparms RETURNS STRING SONAME 'liberymysql.dll';
CREATE FUNCTION eryflushcache RETURNS INTEGER SONAME 'liberymysql.dll';
CREATE FUNCTION erygettrace RETURNS STRING SONAME 'liberymysql.dll';
CREATE FUNCTION erygetversion RETURNS STRING SONAME 'liberymysql.dll';
CREATE FUNCTION eryonlinealter RETURNS INTEGER SONAME 'liberymysql.dll';
CREATE FUNCTION eryviewtablelock RETURNS STRING SONAME 'liberymysql.dll';
CREATE FUNCTION erycleartablelock RETURNS STRING SONAME 'liberymysql.dll';
CREATE FUNCTION erygetsqlcount RETURNS STRING SONAME 'liberymysql.dll';

create database if not exists erydbsys;
use erydbsys;

drop table if exists systable restrict;
drop table if exists syscolumn restrict;

-- SYSTABLE
create table if not exists systable (
	tablename varchar(128),
	`schema` varchar(128),
	objectid int,
	createdate date,
	lastupdate date,
	init int,
	next int,
	numofrows int,
	avgrowlen int,
	numofblocks int,
	autoincrement int
	) engine=erydb comment='SCHEMA SYNC ONLY';

-- SYSCOLUMN
create table if not exists syscolumn (
	`schema` varchar(128),
	tablename varchar(128),
	columnname varchar(128),
	objectid int,
	dictobjectid int,
	listobjectid int,
	treeobjectid int,
	datatype int,
	columnlength int,
	columnposition int,
	lastupdate date,
	defaultvalue varchar(64),
	nullable int,
	scale int,
	prec int,
	autoincrement char(1),
	distcount int,
	nullcount int,
	minvalue varchar(64),
	maxvalue varchar(64),
	compressiontype int,
	nextvalue bigint
	) engine=erydb comment='SCHEMA SYNC ONLY';

