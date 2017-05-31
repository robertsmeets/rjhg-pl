#include "FileCreator.h"

void FileCreator::start(char* filename_exe,char* filename_src_c, char* buffer, int size)
{  
   FILE *infile = fopen(filename_exe, "rb");
   if (infile == NULL) { printf("cannot open %s\n",filename_exe); exit(-1);}
   string filename_src = filename_src_c; 
   int pos = filename_src.find('.');
   string filename_target;
   if (pos == string::npos)
   {
      filename_target = filename_src + ".exe";
   }
   else
   {
      filename_target= filename_src.substr(0,pos)+".exe";
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
   printf("Offset written to file = %ul\n",size);
   fwrite(&size64, 8, 1, outfile);
   fputs("RJHGBC",outfile); 
   fclose(outfile);
}
 
