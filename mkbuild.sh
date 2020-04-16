#/bin/bash

h="$(dirname $0)"
force=false
arch=
bd=
: ${PREFIX:=}
eval set -- $(getopt -o a:b:fp: -l arch:,buildir:,force,prefix: -- "$@") || exit
while :; do
	case "$1" in
	-a|--arch) arch="$2"; shift;;
	-b|--buildir) bd="$2"; shift;;
	-f|--force) force=true;;
	-p|--prefix) PREFIX="$2"; shift;;
	--) shift; break;;
	esac
	shift
done

case "$arch" in
  32) ARCH32=ON; : ${CMAKE_INSTALL_PREFIX:=$HOME/.local32};;
  64) : ${CMAKE_INSTALL_PREFIX:=$HOME/.local64};;
  *) arch=;;
esac
: ${bd:=build${arch}}

mkdir -p "$h/$bd" || exit
cd "$h/$bd" || exit

$force && { rm -r * 2>/dev/null || rm CMakeCache.txt 2>/dev/null; }
test -f CMakeCache.txt -a -f Makefile || \
cmake \
	-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX:=${PREFIX:=$HOME/.local}} \
	-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Debug} \
	-DLIBAFB_DEVEL=${LIBAFB_DEVEL:=OFF} \
	-DWITH_SYSTEMD=${WITH_SYSTEMD:=ON} \
	-DWITHOUT_CYNAGORA=${WITHOUT_CYNAGORA:=OFF} \
	-DWITHOUT_TESTS=${WITHOUT_TESTS:=OFF} \
	-DWITH_AFB_HOOK=${WITH_AFB_HOOK:=ON} \
	-DWITH_SIG_MONITOR_SIGNALS=${WITH_SIG_MONITOR_SIGNALS:=ON} \
	-DWITH_SIG_MONITOR_FOR_CALL=${WITH_SIG_MONITOR_FOR_CALL:=ON} \
	-DWITH_SIG_MONITOR_TIMERS=${WITH_SIG_MONITOR_TIMERS:=ON} \
	-DWITH_AFB_TRACE=${WITH_AFB_TRACE:=ON} \
	-DWITH_SUPERVISION=${WITH_SUPERVISION:=ON} \
	-DWITH_DBUS_TRANSPARENCY=${WITH_DBUS_TRANSPARENCY:=ON} \
	-DWITH_DYNAMIC_BINDING=${WITH_DYNAMIC_BINDING:=ON} \
	-DWITH_LIBMICROHTTPD=${WITH_LIBMICROHTTPD:=ON} \
	-DWITH_L4VSOCK=${WITH_L4VSOCK:=OFF} \
	-DWITH_CRED=${WITH_CRED:=ON} \
	-DWITH_REPLY_JOB=${WITH_REPLY_JOB:=ON} \
	-DWITH_ENVIRONMENT=${WITH_ENVIRONMENT:=ON} \
	-DWITH_AFB_DEBUG=${WITH_AFB_DEBUG:=ON} \
	-DWITH_CALL_PERSONALITY=${WITH_CALL_PERSONALITY:=ON} \
	-DWITH_AFB_CALL_SYNC=${WITH_AFB_CALL_SYNC:=ON} \
	-DWITH_LEGACY_CALLS=${WITH_LEGACY_CALLS:=ON} \
	-DWITH_SIG_MONITOR_DUMPSTACK=${WITH_SIG_MONITOR_DUMPSTACK:=ON} \
	-DWITH_FDEV_POLL=${WITH_FDEV_POLL:=OFF} \
	-DWITH_FDEV_EPOLL=${WITH_FDEV_EPOLL:=OFF} \
	-DWITH_FDEV_SELECT=${WITH_FDEV_SELECT:=OFF} \
	-DWITH_FDEV_SYSTEMD=${WITH_FDEV_SYSTEMD:=${WITH_SYSTEMD}} \
	-DWITH_LOCALE_FOLDER=${WITH_LOCALE_FOLDER:=ON} \
	-DWITH_LOCALE_SEARCH_NODE=${WITH_LOCALE_SEARCH_NODE:=ON} \
	-DWITH_FNMATCH=${WITH_FNMATCH:=ON} \
	-DWITH_LIBUUID=${WITH_LIBUUID:=ON} \
	-DWITH_EPOLL=${WITH_EPOLL:=ON} \
	-DWITH_EVENTFD=${WITH_EVENTFD:=ON} \
	-DWITH_CLOCK_GETTIME=${WITH_CLOCK_GETTIME:=ON} \
	-DWITH_RANDOM_R=${WITH_RANDOM_R:=ON} \
	-DWITH_OPENAT=${WITH_OPENAT:=ON} \
	-DWITH_DIRENT=${WITH_DIRENT:=ON} \
	-DWITH_SYS_UIO=${WITH_SYS_UIO:=ON} \
	-DWITH_REALPATH=${WITH_REALPATH:=ON} \
	-DWITH_UNIX_SOCKET=${WITH_UNIX_SOCKET:=ON} \
	-DWITH_THREAD_LOCAL=${WITH_THREAD_LOCAL:=ON} \
	-DWITH_LIBMAGIC=${WITH_LIBMAGIC:=ON} \
	-DARCH32=${ARCH32:=OFF} \
	..

make -j "$@"

