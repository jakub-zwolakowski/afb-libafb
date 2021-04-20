#!/bin/bash
############################################################################
#                                                                          #
#  This file is part of TrustInSoft Analyzer.                              #
#                                                                          #
#    Copyright (C) 2021 TrustInSoft                                        #
#                                                                          #
#  All rights reserved.                                                    #
#                                                                          #
############################################################################

set -eu

force=false
while (( $# >= 1 )) ; do
  case "$1" in
    -f | --force | --clean ) force=true ; shift ;;
    *) echo "ERROR: unknown parameter $1" ; exit 1 ;;
  esac
done
readonly force

readonly top=$(git rev-parse --show-toplevel)
readonly prefix="$top/tis/root"
readonly top_build="$top/tis/build"
readonly log="$top_build/tis_compile_deps.log"

echo "--- build in $top_build"
$force && echo "    remove build dir" && rm -rf "$top_build"
echo "--- installation in $prefix"
$force && echo "    remove installation dir" && rm -rf "$prefix"
echo "--- log results in $log"
rm -f "$log"

prepare() {
  local src_dir="$1" ; shift
  local build_dir="$1" ; shift
  local install="$1" ; shift
  local opts=( "$@" )
  echo "    build in $build_dir"
  mkdir -p "$build_dir" && cd "$build_dir" || exit 2
  echo "    options:" ; printf "      %s\n" "${opts[@]}"
  cmake "../../../$src_dir" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    "${opts[@]}" \
    >> "$log" 2>&1
  make >> "$log" 2>&1
  if $install ; then
    echo "    install in $prefix"
    make install >> "$log" 2>&1
  fi
  if [ -f compile_commands.json ] ; then
    local cc=compile_commands.json
    echo "    remove absolute path in $cc"
    sed -i "s|$top|../../..|g" "$cc"
  fi
  cd "$top" || exit 2
}

prepare_dep() {
  local d="$1"
  echo "=== Prepare $d"
  local build_dir="$top_build/$d"
  local install=true
  local opts=( "-DCMAKE_INSTALL_PREFIX=$prefix" )
  prepare "$d" "$build_dir" $install "${opts[@]}"
}

prepare_dep afb-binding
prepare_dep json-c
prepare_dep check

echo "=== Prepare application"
build_dir="$top_build/build"
install=false
opts=(
  "-DCMAKE_PREFIX_PATH=$prefix"
  "-DWITH_LIBMICROHTTPD=OFF"
  "-DWITHOUT_CYNAGORA=ON"
  "-DWITH_SYSTEMD=OFF"
  "-DWITH_GNUTLS=OFF"
  "-DWITH_DBUS_TRANSPARENCY=OFF"
)
prepare "." "$build_dir" $install "${opts[@]}"

echo ""
echo "Ready to run: tis-analyzer -tis-config-load tis.config --interpreter"
echo ""
