#define MAIN_FILE
#include "Runner.h"

int Runner::compile_run(string filename,bool debug)
{  
   CodeGenerator* cg = new CodeGenerator();
   compile_with_system(cg, filename, debug);
   //
   // start interpreting
   //
   CI_init(cg->getCodeBuffer());
   CI_start(debug);
   return 0;
}

int Runner::compile_run_string(char* buffer,bool debug)
{  
   CodeGenerator* cg = new CodeGenerator();
   glob = new ProgramNode();
   char buffer2[100];
   memcpy(buffer2,buffer,strlen(buffer)+1);
   yy_scan_string(buffer2); 
   int result = yyparse();
   if(debug){glob->print(0);}
   glob->findMain();
   cg->start(glob,NULL, debug);
   //
   // start interpreting
   //
   CI_init(cg->getCodeBuffer());
   /* if (debug) {
      Disassembler d; 
      d.start(cg->getCodeBuffer(),cg->getHere(),NULL);
   } */
   CI_start(debug);
   return 0;
}


void Runner::compile_with_system(CodeGenerator* cg, string filename, bool debug)
{
   glob = new ProgramNode();
   compile("system/helement.src",debug);
   compile("system/hashtable.src",debug);
   compile(filename,debug);
   glob->findMain();
   if(debug){glob->print(0);}
   cg->start(glob,NULL, debug);
   if (debug) {
      Disassembler d; 
      d.start(cg->getCodeBuffer(),cg->getHere(),NULL);
   }
}


void Runner::compile(string filename,bool debug)
{
   FILE *infile = fopen (filename.c_str(), "rt");
   if (infile == NULL)
   {
      printf("Cannot open %s\n",filename.c_str());
      exit(-1);
   }
   extern FILE * yyin;
   char outfilename[100];
   snprintf(outfilename,100,"%s%s",filename.c_str(),".strip");
   FILE* outfile = fopen (outfilename, "w");
   if (outfile == NULL) { printf("cannot open %s\n",outfilename); exit(-1);  }
   stripcmt(infile, outfile);
   fclose(infile);
   fclose(outfile);
   if (debug) {printf("Parsing... %s ...\n",filename.c_str());}
   extern FILE* yyin;
   yyin = fopen(outfilename,"r");
   char errmsg[] = "error";
   char* ptr = (char*)errmsg;
   int result = yyparse();
   fclose(yyin);
   remove(outfilename);
}
