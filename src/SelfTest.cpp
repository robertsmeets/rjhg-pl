#include "SelfTest.h"


void SelfTest::run()
{
	//
	// open test directory
	//
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
}

void SelfTest::Test(char * filename)
{
   char filename_src[256];
   snprintf(filename_src,255,"%s.src",filename);
   char filename_out[256];
   snprintf(filename_out,255,"%s.out",filename);
   FILE* org_stdout = stdout;
   freopen(filename_out,"w",stdout);
   Runner runner;
   runner.compile_run(filename_src,false); 
   // windows: freopen("CON", "w", stdout);
#ifdef _WIN32
 freopen("CON","w",stdout);
#else
 freopen("/dev/tty","w",stdout);
#endif
   char filename_tst[256];
   snprintf(filename_tst,255,"%s.tst",filename);
   if (Compare(filename_out,filename_tst))
   {
       printf("%s PASSED\n",filename);
   }
   else
   {
       printf("%s FAILED\n",filename);
   }
} 

bool SelfTest::Compare(char* one,char* two)
{
 FILE * fp1;
 FILE * fp2;
 char c1[100], c2[100];
 int cmp;
 
 fp1 = fopen(one, "r");
 fp2 = fopen(two, "r");
 
 if(fp1 == NULL)
 {
    printf("Cannot open %s\n",one);
     return false;
}

 if(fp2 == NULL)
 {
    printf("Cannot open %s\n",two);
     return false;
}
  while(true)
  {
     char* r1=       fgets(c1 , 99, fp1) ;
     char* r2=       fgets(c2 , 99, fp2) ;
     if ((r1 == NULL)!= (r2 == NULL))
     { 
        fclose(fp1);
        fclose(fp2);
        return false;
     }
     if (r1 == NULL)
     {
        fclose(fp1);
        fclose(fp2);
        return true;
     } 
   
  if(!((strcmp(c1, c2)) == 0))
  {
        fclose(fp1);
        fclose(fp2);
  return false; 
  }
 }
}
