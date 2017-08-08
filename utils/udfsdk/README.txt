How to use the EryDB UDF SDK

Obtain the MariaDB erydb source code from https://github.com/mariadb-corporation/erydb-server
and follow the pre-requisite and build instructions.

Go into the utils/udfsdk directory.

At this point you can use the ERYDB_add() function template in udfsdk.cpp and udfmysql.cpp
files to create your own function or just try that function as is.
- Make the library
    $ make
- Copy the libudf_mysql.so.1.0.0 and libudfsdk.so.1.0.0 file to /usr/local/erydb/lib on
  every erydb node.
    $ sudo cp libudf_mysql.so.1.0.0 libudfsdk.so.1.0.0 /usr/local/erydb/lib/
- Restart EryDB
    $ eryadmin restartsystem y
- Using the erymysql client add the user defined function, e.g,
    $ erymysql
    > create function erydb_add returns integer soname 'libudf_mysql.so';
    > create function erydb_isnull returns string soname 'libudf_mysql.so';

You should now be able to use the erydb_add(arg1, arg2) and erydb_isnull(arg) functions in the select and/or where clauses
of SQL statements.