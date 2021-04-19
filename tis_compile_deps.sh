#!/bin/bash

set -o nounset

top=$(pwd)

log="$top/tis_compile_deps.log"
echo "--- log results in $log"
rm -f "$log"

prefix="$top/local"
echo "--- installation in $prefix"
force=false

while (( $# >= 1 )) ; do
  case "$1" in
    -f | --force | --clean ) force=true ; shift ;;
    *) echo "ERROR: unknown parameter $1" ; exit 1 ;;
  esac
done

readonly force

# clear local installation
$force && rm -rf local

sed_path() {
  local f="$1"
  echo "    remove absolute path in $f"
  sed -i "s|$top|..|g" "$f"
}

prepare() {
  local d="$1"
  local build_dir="$top/$d-build"
  echo "=== Prepare $d in $build_dir"
  $force && rm -rf "$build_dir"
  mkdir -p "$build_dir" && cd "$build_dir" || exit 2
  cmake "../$d" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX="$prefix" \
           >> "$log" 2>&1
  ( make && make install ) >> "$log" 2>&1
  if [ -f compile_commands.json ] ; then
    sed_path compile_commands.json
  fi
  cd "$top" || exit 2
}

prepare afb-binding
prepare json-c
prepare check

build_dir="$top/build"
echo "=== Prepare application in $build_dir"
$force && rm -rf "$build_dir"
mkdir -p "$build_dir" && cd "$build_dir" || exit 4
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX="$prefix" \
           -DCMAKE_PREFIX_PATH="$prefix" \
           -DWITH_LIBMICROHTTPD=OFF \
           -DWITHOUT_CYNAGORA=ON \
           -DWITH_SYSTEMD=OFF \
           -DWITH_GNUTLS=OFF \
           -DWITH_DBUS_TRANSPARENCY=OFF  >> "$log" 2>&1
make  >> "$log" 2>&1


