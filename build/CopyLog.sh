#!/usr/bin/expect -f
#@echo off
spawn scp /EryDB/genii/mysql/queries/nightly/srvswdev11/go.log root@srvnightly:/tmp/srvbldwin1-genii-queryTester.log

expect {
	-re "authenticity" { send "yes\r" 
						expect {
							-re "word: " { send "$PASSWORD\r" } abort
							-re "passphrase" { send "$PASSWORD\r" } abort
							}
						}
	-re "word: " { send "erydb1\r" } abort
	}
expect "*\r"
expect "\r"