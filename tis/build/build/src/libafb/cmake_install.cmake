# Install script for directory: /home/qba/git/afb-libafb/src/libafb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/tis/build/build/src/libafb/libafb-config.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/libafb.h"
    "/home/qba/git/afb-libafb/src/libafb/afb-extension.h"
    "/home/qba/git/afb-libafb/src/libafb/afb-v4.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/core" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/core/afb-api-common.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-api-v3.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-api-v4.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-apiname.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-apiset.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-auth.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-calls.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-common.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-cred.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-data-array.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-data.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-error-text.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-ev-mgr.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-evt.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-global.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-hook-flags.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-hook.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-jobs.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-json-legacy.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-perm.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-permission-text.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-req-common.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-req-v3.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-req-v4.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-sched.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-session.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-sig-monitor.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-string-mode.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-token.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-type-internal.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-type-predefined.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-type.h"
    "/home/qba/git/afb-libafb/src/libafb/core/afb-v4-itf.h"
    "/home/qba/git/afb-libafb/src/libafb/core/containerof.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-core.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/utils" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/utils/expand-json.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/expand-vars.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/globmatch.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/globset.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/json-locator.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/jsonstr.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/locale-root.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/lockany.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/namecmp.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/path-search.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/pearson.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/sha1.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/u16id.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/uuid.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/websock.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/wrap-json.h"
    "/home/qba/git/afb-libafb/src/libafb/utils/ws.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-utils.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/sys" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/sys/ev-mgr.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/process-name.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/subpath.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/systemd.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/verbose.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-buf.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-cond.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-dynlib.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-endian.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-epoll.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-errno.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-mutex.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-poll.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-realpath.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-rwlock.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-socket.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-thread.h"
    "/home/qba/git/afb-libafb/src/libafb/sys/x-uio.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-sys.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/http" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/http/afb-hreq.h"
    "/home/qba/git/afb-libafb/src/libafb/http/afb-hsrv.h"
    "/home/qba/git/afb-libafb/src/libafb/http/afb-hswitch.h"
    "/home/qba/git/afb-libafb/src/libafb/http/afb-method.h"
    "/home/qba/git/afb-libafb/src/libafb/http/afb-websock.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-http.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/apis" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/apis/afb-api-dbus.h"
    "/home/qba/git/afb-libafb/src/libafb/apis/afb-api-so-v3.h"
    "/home/qba/git/afb-libafb/src/libafb/apis/afb-api-so-v4.h"
    "/home/qba/git/afb-libafb/src/libafb/apis/afb-api-so.h"
    "/home/qba/git/afb-libafb/src/libafb/apis/afb-api-ws.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-apis.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/misc" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-autoset.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-debug.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-monitor.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-socket.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-supervision.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-supervisor.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-systemd.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-trace.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-watchdog.h"
    "/home/qba/git/afb-libafb/src/libafb/misc/afb-ws.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-misc.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/wsapi" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/wsapi/afb-proto-ws.h"
    "/home/qba/git/afb-libafb/src/libafb/wsapi/afb-stub-ws.h"
    "/home/qba/git/afb-libafb/src/libafb/wsapi/afb-wsapi.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-wsapi.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/wsj1" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/wsj1/afb-ws-json1.h"
    "/home/qba/git/afb-libafb/src/libafb/wsj1/afb-wsj1.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-wsj1.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/extend" TYPE FILE FILES
    "/home/qba/git/afb-libafb/src/libafb/extend/afb-extend.h"
    "/home/qba/git/afb-libafb/src/libafb/extend/afb-extension.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-extend.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb/tls" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/tls/tls.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libafb" TYPE FILE FILES "/home/qba/git/afb-libafb/src/libafb/afb-tls.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/qba/git/afb-libafb/tis/build/build/src/libafb/libafb.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so.4.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so.4"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/qba/git/afb-libafb/tis/build/build/src/libafb/libafb.so.4.0"
    "/home/qba/git/afb-libafb/tis/build/build/src/libafb/libafb.so.4"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so.4.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so.4"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/qba/git/afb-libafb/tis/build/build/src/libafb/libafb.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libafb.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/qba/git/afb-libafb/tis/build/build/src/libafb/libafb.pc")
endif()

