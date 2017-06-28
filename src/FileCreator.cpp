#include "FileCreator.h"

#ifdef _WIN32
   #define XEXTENSION ".exe"
#else
   #define XEXTENSION ""
#endif

void FileCreator::start(char* filename_src_c, char* buffer, int size)
{  
   string filename_exe = string("interpret") + string(XEXTENSION); 
   FILE *infile = fopen(filename_exe.c_str(), "rb");
   if (infile == NULL) { printf("cannot open %s\n",filename_exe.c_str()); exit(-1);}
   string filename_src = filename_src_c; 
   int pos = filename_src.find('.');
   string filename_target;
   if (pos == string::npos)
   {
      filename_target = filename_src + XEXTENSION;
   }
   else
   {
      filename_target= filename_src.substr(0,pos) + XEXTENSION;
   }
   FILE *outfile = fopen(filename_target.c_str(),"wb");
   char buf[BUFSIZ];
   size_t sz;
   while (sz = fread(buf, 1, BUFSIZ, infile)) {
        fwrite(buf, 1, sz, outfile);
   }
   fclose(infile);
   //
   // add the bytecode to the file
   // 
   fwrite(buffer, 1, size, outfile);
   //
   // 
   uint64_t size64 = size;
   fwrite(&size64, 8, 1, outfile);
   fputs("RJHGBC",outfile); 
   fclose(outfile);
#ifndef _WIN32
   chmod(filename_target.c_str(),S_IRWXU);
#endif
   printf("Written executable file %s\n",filename_target.c_str());
}
 
