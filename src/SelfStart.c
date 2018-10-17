#include "SelfStart.h"

void CI_start(bool); 
void CI_init(char*);

int S_start(char* filename)
{
   char* filename_open = S_shorten(filename);
   FILE *infile = fopen (filename_open, "rt");
   if (!infile) { printf("cannot open %s\n",filename); }
   char* buffer = (char*) GC_MALLOC(14);
   size_t result = fread(buffer, 1, 4, infile);
   if(strncmp(buffer,XMAGIC,strlen(XMAGIC)) != 0)
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
      return -1;
   }
   uint64_t* uptr = (uint64_t*)buffer;
   uint64_t offset = *uptr;
   fseek(infile, -14-offset,SEEK_END);
   result = fread(buffer,1,6,infile);
   if(strncmp(buffer,"RJHGPL",6) != 0)
   {
      return -1;
   }
   char* bcbuffer = (char*) GC_MALLOC(offset);
   fseek(infile, -14-offset,SEEK_END);
   result = fread(bcbuffer,1,offset,infile);
   fclose(infile);
   CI_init(bcbuffer);
   CI_start(false); 
   return 0;
}
  
int lower_case (int c)
{
  return tolower(c);
}

 
char* S_shorten(char* filename)
{
   char* filename_open = filename;
#ifdef _WIN32
   int len = strlen(filename_open);
   if (len >= 5)
   {
       char* endstring = GC_MALLOC(len-4);
       strncpy(endstring,filename_open,len - 4);
       //transform(endstring.begin(), endstring.end(), endstring.begin(), lower_case);
       if (endstring != ".exe")
       {
          //filename_open += ".exe";
       }
   } 
   else
   {
      //filename_open += ".exe";
   }
#endif
   return filename_open;
}
