#!/bin/bash
#
# $Id: module_installer.sh 421 2007-04-05 15:46:55Z dhill $
#
# Setup the Custom OS files during a System install on a module
#
#
# append erydb OS files to Linux OS file
#
#

prefix=/usr/local
installdir=$prefix/erydb
rpmmode=install
user=$USER
if [ -z "$user" ]; then
	user=root
fi
quiet=0
shiftcnt=0

for arg in "$@"; do
	if [ $(expr -- "$arg" : '--prefix=') -eq 9 ]; then
		prefix="$(echo $arg | awk -F= '{print $2}')"
		installdir=$prefix/erydb
		((shiftcnt++))
	elif [ $(expr -- "$arg" : '--rpmmode=') -eq 10 ]; then
		rpmmode="$(echo $arg | awk -F= '{print $2}')"
		((shiftcnt++))
	elif [ $(expr -- "$arg" : '--installdir=') -eq 13 ]; then
		installdir="$(echo $arg | awk -F= '{print $2}')"
		prefix=$(dirname $installdir)
		((shiftcnt++))
	elif [ $(expr -- "$arg" : '--user=') -eq 7 ]; then
		user="$(echo $arg | awk -F= '{print $2}')"
		((shiftcnt++))
	elif [ $(expr -- "$arg" : '--quiet') -eq 7 ]; then
		quiet=1
		((shiftcnt++))
	elif [ $(expr -- "$arg" : '--port') -eq 6 ]; then
		mysqlPort="$(echo $arg | awk -F= '{print $2}')"
		((shiftcnt++))
	elif [ $(expr -- "$arg" : '--module') -eq 8 ]; then
		module="$(echo $arg | awk -F= '{print $2}')"
		((shiftcnt++))
	fi
done
shift $shiftcnt

if [ $installdir != "/usr/local/erydb" ]; then
	export ERYDB_INSTALL_DIR=$installdir
	export PATH=$ERYDB_INSTALL_DIR/bin:$ERYDB_INSTALL_DIR/mysql/bin:/bin:/usr/bin
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ERYDB_INSTALL_DIR/lib:$ERYDB_INSTALL_DIR/mysql/lib/mysql
else
	export ERYDB_INSTALL_DIR=$installdir
fi

cloud=`$ERYDB_INSTALL_DIR/bin/getConfig Installation Cloud`
if [ $cloud = "amazon-ec2" ] || [ $cloud = "amazon-vpc" ]; then
	cp $ERYDB_INSTALL_DIR/local/etc/credentials $HOME/.aws/. > /dev/null 2>&1

	if [ $module = "pm" ]; then
		if test -f $ERYDB_INSTALL_DIR/local/etc/pm1/fstab ; then
			echo "Setup fstab on Module"
        		if [ $user = "root" ]; then
				touch /etc/fstab
				rm -f /etc/fstab.erydbSave
				cp /etc/fstab /etc/fstab.erydbSave
				cat $ERYDB_INSTALL_DIR/local/etc/pm1/fstab >> /etc/fstab
			else
                                sudo touch /etc/fstab
				sudo chmod 666 /etc/fstab
                                sudo rm -f /etc/fstab.erydbSave
                                sudo cp /etc/fstab /etc/fstab.erydbSave
                                sudo cat $ERYDB_INSTALL_DIR/local/etc/pm1/fstab >> /etc/fstab
			fi
		fi
	fi
fi

test -f $ERYDB_INSTALL_DIR/post/functions && . $ERYDB_INSTALL_DIR/post/functions

mid=`module_id`

yum install -y  net-snmp-agent-libs boost-system boost-date-time boost-devel boost-filesystem boost-thread boost-regex lz4 zlib snappy rsync expect readline ncurses nmap lsof libxml2 jemalloc

#if um, cloud, separate system type, external um storage, then setup mount
if [ $module = "um" ]; then
	if [ $cloud = "amazon-ec2" ] || [ $cloud = "amazon-vpc" ]; then
		systemtype=`$ERYDB_INSTALL_DIR/bin/getConfig Installation ServerTypeInstall`
		if [ $systemtype = "1" ]; then
			umstoragetype=`$ERYDB_INSTALL_DIR/bin/getConfig Installation UMStorageType`
			if [ $umstoragetype = "external" ]; then
				echo "Setup UM Volume Mount"
				device=`$ERYDB_INSTALL_DIR/bin/getConfig Installation UMVolumeDeviceName$mid`
				mkdir -p $ERYDB_INSTALL_DIR/mysql/db > /dev/null 2>&1
				sudo mount $device $ERYDB_INSTALL_DIR/mysql/db -t ext2 -o noatime,nodiratime,noauto,user
				chown mysql:mysql -R $ERYDB_INSTALL_DIR/mysql > /dev/null 2>&1
			fi
		fi
	fi
fi

#if pm, create dbroot directories
if [ $module = "pm" ]; then
	numdbroots=`$ERYDB_INSTALL_DIR/bin/getConfig SystemConfig DBRootCount`
	for (( id=1; id<$numdbroots+1; id++ )); do
		mkdir -p $ERYDB_INSTALL_DIR/data$id > /dev/null 2>&1
		chmod 755 $ERYDB_INSTALL_DIR/data$id
	done
fi

echo "Setup rc.local on Module"
if [ $EUID -eq 0 -a -f $ERYDB_INSTALL_DIR/local/rc.local.erydb ]; then
	if [ $user = "root" ]; then
		touch /etc/rc.local
		rm -f /etc/rc.local.erydbSave
		cp /etc/rc.local /etc/rc.local.erydbSave
		cat $ERYDB_INSTALL_DIR/local/rc.local.erydb >> /etc/rc.local
	else
		sudo touch /etc/rc.local
		sudo rm -f /etc/rc.local.erydbSave
		sudo cp /etc/rc.local /etc/rc.local.erydbSave
		sudo cat $ERYDB_INSTALL_DIR/local/rc.local.erydb >> /etc/rc.local
	fi
fi

 
	echo "Setup /etc/profile.d/erydb.sh on Module for env"
	bashFile=/etc/profile.d/erydb.sh
	touch ${bashFile}

	echo "export ERYDB_INSTALL_DIR=$ERYDB_INSTALL_DIR" >> ${bashFile}
	echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ERYDB_INSTALL_DIR/lib:$ERYDB_INSTALL_DIR/mysql/lib/mysql" >> ${bashFile}
 

plugin=`$ERYDB_INSTALL_DIR/bin/getConfig SystemConfig DataFilePlugin`
if [ -n "$plugin" ]; then
	echo "Setup .bashrc on Module for local-query"

	setenv=`$ERYDB_INSTALL_DIR/bin/getConfig SystemConfig DataFileEnvFile`

	eval userhome=~$user
	bashFile=$userhome/.bashrc
	touch ${bashFile}

	echo " " >> ${bashFile}
	echo ". $ERYDB_INSTALL_DIR/bin/$setenv" >> ${bashFile}
fi

# if mysqlrep is on and module has a my.cnf file, upgrade it

MySQLRep=`$ERYDB_INSTALL_DIR/bin/getConfig Installation MySQLRep`
if [ $MySQLRep = "y" ]; then
	if test -f $ERYDB_INSTALL_DIR/mysql/my.cnf ; then
		echo "Run Upgrade on my.cnf on Module"
		$ERYDB_INSTALL_DIR/bin/mycnfUpgrade > /tmp/mycnfUpgrade.log 2>&1
	fi
fi

if test -f $ERYDB_INSTALL_DIR/mysql/my.cnf ; then
	echo "Run Mysql Port update on my.cnf on Module"
	$ERYDB_INSTALL_DIR/bin/mycnfUpgrade $mysqlPort > /tmp/mycnfUpgrade_port.log 2>&1
fi

# if um, run mysql install scripts
if [ $module = "um" ]; then
	echo "Run post-mysqld-install"
	$ERYDB_INSTALL_DIR/bin/post-mysqld-install > /tmp/post-mysqld-install.log 2>&1
	if [ $? -ne 0 ]; then
	    echo "ERROR: post-mysqld-install failed: check /tmp/post-mysqld-install.log"
	    exit 1
	fi
	echo "Run post-mysql-install"
	$ERYDB_INSTALL_DIR/bin/post-mysql-install > /tmp/post-mysql-install.log 2>&1
        if [ $? -ne 0 ]; then
            echo "ERROR: post-mysql-install failed: check /tmp/post-mysql-install.log"
            exit 1
	fi
fi

$ERYDB_INSTALL_DIR/bin/syslogSetup.sh check > /tmp/syslogSetup-check.log 2>&1
if [ $? -ne 0 ]; then
	# try setup again
	$ERYDB_INSTALL_DIR/bin/syslogSetup.sh install > /tmp/syslogSetup-install.log 2>&1
	if [ $? -ne 0 ]; then
		echo "WARNING: syslogSetup.sh check failed: check /tmp/syslogSetup-check.log"
       		exit 2
	fi
fi
 

echo "!!!Module Installation Successfully Completed!!!"

exit 0
