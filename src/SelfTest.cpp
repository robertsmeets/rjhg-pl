#include "SelfTest.h"


void SelfTest::run()
{
	//
	// open test directory
	//

   printf("SELFTEST\n");
   printf("--------------------------------\n");
   string path = "test/list.txt";
   FILE *infile;
   infile = fopen(path.c_str(), "r");       
   if (infile == NULL)
   {
      printf("file test/list.txt not found\n");
      return;
   } 
   char line[256];
   while (fgets(line, sizeof(line), infile)) {
       line[strcspn(line, "\r\n")] = 0;
       char line2[100];
       snprintf(line2,sizeof(line2),"test/%s", line);
            Test(line2);
   }
   fclose(infile);
   printf("--------------------------------\n");
}

void SelfTest::Test(char * filename)
{
   printf("Testing <%s>\n",filename);
   FILE* org_stdout = stdout;
   stdout = freopen("my_log.txt", "w", stdout);
   Runner runner;
   runner.compile_run(filename); 
   stdout = org_stdout;
} 
