#include "SelfStart.h"

#ifdef _WIN32
   #define XMAGIC "MZ"
#else
   #define XMAGIC "\x7F""ELF" 
#endif

int SelfStart::start(char* filename)
{
   FILE *infile = fopen (filename, "rt");
   if (!infile) { printf("cannot open %s\n",filename); }
   char* buffer = (char*) GC_MALLOC(14);
   fread(buffer, 1, 4, infile);
   if(strncmp(buffer,XMAGIC,string(XMAGIC).length()) != 0)
   {  
      printf("<%s> does not appear to be an executable\n", filename);
      return -1;
   }
   //
   // ELF binary:  0x7F followed by ELF(45 4c 46) in ASCII; these four bytes constitute the magic number. 
   //
   fseek(infile, -14 , SEEK_END);
   fread(buffer,1,14,infile);
   if(strncmp(buffer+8,"RJHGBC",6) != 0)
   {
      printf("Indicator RJHGBC NOT found\n");
      return -1;
   }
   uint64_t* uptr = (uint64_t*)buffer;
   uint64_t offset = *uptr;
   fseek(infile, -14-offset,SEEK_END);
   fread(buffer,1,6,infile);
   if(strncmp(buffer,"RJHGPL",6) != 0)
   {
      printf("Indicator RJHGPL NOT found\n");
      return -1;
   }
   char* bcbuffer = (char*) GC_MALLOC(offset);
   fseek(infile, -14-offset,SEEK_END);
   fread(bcbuffer,1,offset,infile);
   fclose(infile);
   CInterpreter i(bcbuffer,NULL);
   i.start(false); 
   return 0;
}
 
