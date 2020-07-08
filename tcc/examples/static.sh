#!/bin/sh
cc -static -I../libtcc/ -L.. libtcc_test.c -Wl,-Bstatic ../libtcc.lib
./a -I../include -L../lib 
