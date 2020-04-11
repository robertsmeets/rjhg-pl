#ifndef SELFSTART_H_
#define SELFSTART_H_ 

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "gc.h"
#include <string.h>
#include "CInterpreter.h"

#if defined(__linux__)
   #define XMAGIC "\x7F""ELF" 
#else
   #define XMAGIC "MZ"
#endif

   char* S_shorten(char*);

#endif

