#include "SelfStart.h"

int SelfStart::start(char* filename)
{
   string filename_open = shorten(filename);
   FILE *infile = fopen (filename_open.c_str(), "rt");
   if (!infile) { printf("cannot open %s\n",filename); }
   char* buffer = (char*) GC_MALLOC(14);
   size_t result = fread(buffer, 1, 4, infile);
   if(strncmp(buffer,XMAGIC,string(XMAGIC).length()) != 0)
   {  
      printf("<%s> does not appear to be an executable\n", filename);
      return -1;
   }
   //
   // ELF binary:  0x7F followed by ELF(45 4c 46) in ASCII; these four bytes constitute the magic number. 
   //
   fseek(infile, -14 , SEEK_END);
   result = fread(buffer,1,14,infile);
   if(strncmp(buffer+8,"RJHGBC",6) != 0)
   {
      printf("Indicator RJHGBC NOT found\n");
      return -1;
   }
   uint64_t* uptr = (uint64_t*)buffer;
   uint64_t offset = *uptr;
   fseek(infile, -14-offset,SEEK_END);
   result = fread(buffer,1,6,infile);
   if(strncmp(buffer,"RJHGPL",6) != 0)
   {
      printf("Indicator RJHGPL NOT found\n");
      return -1;
   }
   char* bcbuffer = (char*) GC_MALLOC(offset);
   fseek(infile, -14-offset,SEEK_END);
   result = fread(bcbuffer,1,offset,infile);
   fclose(infile);
   CInterpreter i(bcbuffer,NULL);
   i.start(false); 
   return 0;
}
  
int lower_case (int c)
{
  return tolower(c);
}

 
string SelfStart::shorten(char* filename)
{
   string filename_open = string(filename);
#ifdef _WIN32
   int len = filename_open.length();
   if (len >= 5)
   {
       string endstring = filename_open.substr(len - 4,len);
       transform(endstring.begin(), endstring.end(), endstring.begin(), lower_case);
       if (endstring != ".exe")
       {
          filename_open += ".exe";
       }
   } 
   else
   {
      filename_open += ".exe";
   }
#endif
   return filename_open;
}
