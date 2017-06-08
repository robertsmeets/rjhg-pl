/**
 *
 * main entry point
 *
 */
#include <stdio.h>
#include "SelfStart.h"

int main(int argc, char* argv[]) {
   //
   // workaround for a bug in the Eclipse console
   //
   setvbuf(stdout, NULL, _IONBF, 0);
   GC_INIT();
   SelfStart s;
   return s.start(argv[0]); 
}
