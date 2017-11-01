/**
 *
 * main entry point
 *
 */
#include "gc.h"
#include <stdio.h>

extern "C" void S_start(char*);

int main(int argc, char* argv[]) {
   //
   // workaround for a bug in the Eclipse console
   //
   setvbuf(stdout, NULL, _IONBF, 0);
   GC_INIT();
   S_start(argv[0]);
   return(0);
}
