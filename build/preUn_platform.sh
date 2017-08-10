rpmmode=upgrade
if  [ "$1" -eq "$1" 2> /dev/null ]; then
	if [ $1 -ne 1 ]; then
		rpmmode=erase
	fi
else
	rpmmode=erase
fi

if [ $rpmmode = erase ]; then
	test -x /usr/local/erydb/bin/pre-uninstall && /usr/local/erydb/bin/pre-uninstall
fi

exit 0

