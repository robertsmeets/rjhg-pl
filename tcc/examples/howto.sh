#!/bin/sh
cc -I../libtcc/ -L../ libtcc_test.c ../libtcc.dll
./a -I../include -L../lib/
