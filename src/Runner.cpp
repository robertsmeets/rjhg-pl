#define MAIN_FILE
#include "Runner.h"

int Runner::compile_run(string filename,bool debug)
{  
   CodeGenerator* cg = new CodeGenerator();
   compile_with_system(cg, filename, debug);
   //
   // start interpreting
   //
   CInterpreter i(cg->getCodeBuffer(),NULL);
   i.start(debug);
   return 0;
}

void Runner::compile_with_system(CodeGenerator* cg, string filename, bool debug)
{
   glob = new ProgramNode();
   compile("system/helement.src",debug);
   compile("system/hashtable.src",debug);
   compile(filename,debug);
   cg->start(glob,NULL, debug);
   if (debug) {
      Disassembler d; 
      d.start(cg->getCodeBuffer(),cg->getHere(),NULL);
   }
}


void Runner::compile(string filename,bool debug)
{
   if (debug) {printf("Removing comments %s ...\n",filename.c_str());}
   FILE *infile = fopen (filename.c_str(), "rt");
   extern FILE * yyin;
   char outfilename[100];
   snprintf(outfilename,100,"%s%s",filename.c_str(),".strip");
   FILE* outfile = fopen (outfilename, "w");
   if (!infile) { printf("cannot open %s\n",filename.c_str()); }
   if (!outfile) { printf("cannot open %s\n",outfilename);  }
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
   if(debug){glob->print(0);}

}
