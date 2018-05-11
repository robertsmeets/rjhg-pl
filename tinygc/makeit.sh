#!/bin/sh
#-c means to create an intermediary object file, rather than an executable.
cc -c -o tinygc.o tinygc.c -DGC_FASTCALL= -DGC_CLIBDECL=
#
#This creates the static library. r means to insert with replacement, c means to create a new archive, and s means to write an index. As always, see the man page for more info.
#
ar rcs libgc-lib.a tinygc.o
