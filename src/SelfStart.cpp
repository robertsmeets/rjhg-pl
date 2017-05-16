#include "SelfStart.h"
int SelfStart::start(char* filename)
{
   printf("Attempting selfstart... %s\n",filename);
   FILE *infile = fopen (filename, "rt");
   if (!infile) { printf("cannot open %s\n",filename); }
   char buffer [4096];
   fread(buffer, 1, 4096, infile);
   if ((buffer[0] != 'M') || (buffer[1] != 'Z'))
   {
       printf("MZ header not found\n");
       return -1;
   }
   unsigned char offset = (buffer[0x3c] & 0xff) - 1;
   if (((buffer[offset] & 0xff) != 'P') || ((buffer[offset+1 ] & 0xff) != 'E'))
   {
       printf("PE header not found offset = 0x%x <%c> <%c> \n",offset,buffer[offset] & 0xff,buffer[offset+1] & 0xff);
       return -1;
   } 
   unsigned int number_of_sections = buffer[offset+6] & 0xff;
   unsigned int image_header = offset + 0x178 - 0x7f; 
   printf("Number of sections = %d\n",number_of_sections);
   unsigned int max = 0;
   unsigned int end_of_file = 0;
   for (unsigned int i=0; i < number_of_sections ; i++)
   {
       unsigned int offset = image_header + i * 40;
       uint32_t* p = (uint32_t*) (buffer + offset + 0x10);
       unsigned int section_size =  (*p);
       p = (uint32_t*) (buffer + offset + 0x14);
       unsigned int section_location =  (*p);
       printf("section location = 0x%x\n",section_location);
       printf("section size = 0x%x\n",section_size);
       printf("---------------------------\n");
       if (section_location > max)
       {
          max = section_location;
          end_of_file = section_location + section_size;
       }
   }
   printf("Predicted end of file = %d 0x%x\n",end_of_file,end_of_file);
}
 
