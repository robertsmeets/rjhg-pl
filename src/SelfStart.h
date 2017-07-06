#ifndef SELFSTART_H_
#define SELFSTART_H_ 

#include "stdio.h"
#include "stdint.h"
#include "gc.h"
#include <string.h>
#include "CInterpreter.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#ifdef _WIN32
   #define XMAGIC "MZ"
#else
   #define XMAGIC "\x7F""ELF" 
#endif

using namespace std;


class SelfStart {

public:
   int start(char*);
   string shorten(char*);
};
#endif

