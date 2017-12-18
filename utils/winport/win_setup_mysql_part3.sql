use erydbsys;
select eryonlinealter(
	'alter table syscolumn add (compressiontype int)') as xxx;
alter table syscolumn add (compressiontype int);
update syscolumn set compressiontype=0 where compressiontype is null;

