#! /bin/bash

mkdir -p chroot
cd chroot/
mkdir -p bin etc lib lib64 var home usr/bin usr/lib usr/include
cp /bin/sh bin
cp /lib/x86_64-linux-gnu/libc.so.6 lib
cp /lib64/ld-linux-x86-64.so.2 lib64
#cp /lib/ld-linux.so.2 lib
cp /lib/x86_64-linux-gnu/libpthread.so.0 lib 
cp /lib/x86_64-linux-gnu/libdl.so.2 lib
cp /lib/x86_64-linux-gnu/libutil.so.1 lib
cp /usr/lib/x86_64-linux-gnu/libssl.so.1.0.0 lib
cp /usr/lib/x86_64-linux-gnu/libcrypto.so.1.0.0 lib
cp /lib/x86_64-linux-gnu/libz.so.1 lib
cp /lib/x86_64-linux-gnu/libc.so.6 lib
cp /lib/x86_64-linux-gnu/libgcc_s.so.1 lib
cp /lib/x86_64-linux-gnu/libm.so.6 lib
cp /usr/bin/python usr/bin
cp -R /usr/lib/pyshared usr/lib
cp -R /usr/lib/python2.7 usr/lib
cp -R /usr/include/python2.7 usr/include

