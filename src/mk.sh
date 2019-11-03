#!/bin/bash
rm -f CMakeCache.txt
rm -rf CMakeFiles/
cmake -G "MSYS Makefiles"
