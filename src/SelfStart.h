#ifndef SELFSTART_H_
#define SELFSTART_H_ 

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "gc.h"
#include <string.h>
#include "CInterpreter.h"

#ifdef _WIN32
   #define XMAGIC "MZ"
#else
   #define XMAGIC "\x7F""ELF" 
#endif

   //int S_start(char*);
   char* S_shorten(char*);

#endif

