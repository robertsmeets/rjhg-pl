#!/bin/sh
# make the libffi sample program
gcc -I../libffi-3.2.1/i686-pc-mingw32/include -L../libffi-3.2.1/i686-pc-mingw32/.libs simplex3.c -llibffi
