# erydb_rep 版本
基于MariaDB的分布式列式数据库，并行MPP扩展，支持GFS HDFS存储及外部存储，百亿数据秒查 秒级汇总，mysql数据库自动多主备份，内部存储下自动副本支持，读写分离等

## install prerequisites
To install erydb binary you will need:

  * a CentOS7  linux host configured for software development

Along with a working C++ compiler and GNU software development tools you will need the following extra packages:

  * expect
  * zlib-devel
  * ncurses-devel
  * libxml2-devel
  * readline-devel
ex:
yum install -y  net-snmp-agent-libs boost-system boost-date-time boost-filesystem boost-thread boost-regex lz4 zlib snappy rsync expect readline ncurses nmap lsof libxml2 

## install steps
### 1、在要安装的所有主机上先做好相互的SSH免密登录
### 2、下载包到 /root目录
### 3、tar xf erydb-5.0-0.bin.tar.gz -C /usr/local
### 4、cd /usr/local/erydb && ./bin/postConofingure

##
localedef -f UTF-8 -i zh_CN zh_CN.UTF8 && localedef --list-archive 


yum install epel-release -y gdb-gdbserver
rpm -ivh http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el7.rf.x86_64.rpm
yum clean all && yum makecache
yum groupinstall "Development tools" "Server Platform Development" -y

yum install -y aclocal autoconf automake  gdb-gdbserver rsync thrift.x86_64 thrift-devel.x86_64 expect net-snmp net-snmp-utils net-snmp-libs net-snmp-devel net-snmp-agent-libs readline readline-devel readline-static  libxml2-devel lz4 lz4-devel libpcap nmap lsof socat ncurses-devel ncurses-libs ncurses  jemalloc-devel jemalloc pcre pcre-devel pcre-static pcre-tools  pcre2 pcre2-devel pcre2-static pcre2-tools libnotify-devel libnotify libxml2 lzo lzo-devel lzo1 lzo1-devel lzop lzma lzma-libs  lzma-devel xz xz-devel xz-libs xz-lzma-compat xz-compat-libs lbzip2 lbzip2-utils bzip2-devel bzip2-libs libzip-devel snappy-devel snappy libaio libaio-devel libstemmer libstemmer-devel libevent libevent-devel msgpack msgpack-devel boost boost-graph boost-build boost-devel cracklib-devel cracklib perl-ZMQ-LibZMQ3  ncompress pxz  flex byacc  automake libtool   libsysfs-devel libsysfs cmake ncurses-devel
 
##################################cmake #debug Compile erydb#############
#debug
chmod +x -R *

cd erymysql 
sed -i -e "s|\r||" BUILD/autorun.sh
BUILD/autorun.sh
export CXXFLAGS="  -w  "
export CFLAGS=" -w   "
export LDFLAGS=" "
sed -i -e "s|\r||g" configure
sed -i -e "s|\r||g" include/probes_mysql.d*
sed -i -e "s|\r||g" sql/share/errmsg-utf8.txt
sed -i -e "s|\r||g" storage/tokudb/PerconaFT/third_party/xz-4.999.9beta/build-aux/config.guess

find ./ -name CMakeFiles |xargs rm -rf 
find ./ -name config.sub|xargs  sed -i -e "s|\r||g" 
find ./ -name config.guess|xargs  sed -i -e "s|\r||g" 
find ./ -name CMakeLists.txt |xargs  sed -i -e "s|\r||g" 
find ./ -name missing |xargs  sed -i -e "s|\r||g" 
find ./ -name config.rpath|xargs  sed -i -e "s|\r||g" 


#Debug Release RelWithDebInfo MinSizeRel
find ./ -name CMakeFiles |xargs rm -rf 
export CXXFLAGS="  -w  "
export CFLAGS=" -w   "
export LDFLAGS=" "
./configure  --with-debug=1   -DDBUG_ON=1 
./configure -DCMAKE_BUILD_TYPE=Release 

make && make install 


cd ../erydb-5.1.1
export CXXFLAGS="  -w   -DBRM_VERBOSE -DERYDB_SM_DEBUG -DPROFILE"
export CFLAGS=" -w    -DBRM_VERBOSE -DERYDB_SM_DEBUG -DPROFILE"
export LDFLAGS=" "
cmake . -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_INSTALL_PREFIX=/usr/local/erydb 
cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/erydb 
make && make install 
##################################################end debug  #############




<h1>交流</h1>
QQ：34954344 <br>
邮箱：hans511002@sohu.com <br>
