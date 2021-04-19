# Summary

```
$ git clone ssh://git@git.trust-in-soft.com:7999/use/afb-libafb.git
$ cd afb-libafb/
$ git co tis
$ git submodule update --init --recursive

$ cd afb-binding/
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX=$PWD/../../local
$ make
$ make install
$ cd ../..

$ cd json-c/
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX=$PWD/../../local
$ make
$ make install
$ cd ../..

$ cd check
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX=$PWD/../../local
$ make
$ make install
$ cd ../..

$ mkdir build
$ cd build
$ cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX=$PWD/../local \
           -DCMAKE_PREFIX_PATH=$PWD/../local \
           -DWITH_LIBMICROHTTPD=OFF \
           -DWITHOUT_CYNAGORA=ON \
           -DWITH_SYSTEMD=OFF \
           -DWITH_GNUTLS=OFF \
           -DWITH_DBUS_TRANSPARENCY=OFF
$ make
$ make install
$ cd ..

$ tis-analyzer -tis-config-load tis/tis.config --interpreter
```

# On-going work


```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX=$PWD/../local \
           -DCMAKE_PREFIX_PATH=$PWD/../local \
           -DWITH_LIBMICROHTTPD=OFF \
           -DWITHOUT_CYNAGORA=ON \
           -DWITH_SYSTEMD=OFF \
           -DWITH_GNUTLS=OFF \
           -DWITH_DBUS_TRANSPARENCY=OFF \
$ make
$ make install
$ cd ..
```

```
$ cd build
$ make test
Running tests...
Test project <USE_path>/afb-libafb/build
No tests were found!!!
```

Another missing dependency for test is `libcheck`:
```
$ git submodule add https://github.com/libcheck/check.git
$ cd check
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_INSTALL_PREFIX=$PWD/../../local \
$ make
$ make install
$ cd ../..
```


```
$ cd build
<re-run the cmake command>
$ make test
4% tests passed, 24 tests failed out of 25
```

Seems to have a problem here.

Let's switch to the analysis then:

```
$ tis-analyzer -tis-config-load tis/tis.config --interpreter
```

## Modifications

### afb-libafb

In `src/libafb/utils/wrap-json.c`, pb with annotation `/*@fallthrough@*/`:
-> source code modification (TODO: rather use `custom-annot-char`?)


### libcheck

Using stubs for `libcheck` (see `tis/tis_check_lib.c`).

## Analyzing libcheck source code

When I tried to use the `libcheck` source code, I tried to configure more
things:
```
$ cmake <...>
           -DHAVE_GETTIMEOFDAY=1 \
           -DHAVE_SYS_TIME_H=1 \
           -DHAVE_FORK=0 -DHAVE_SIGACTION=0
```

I stopped on many problems with `time` functions...

