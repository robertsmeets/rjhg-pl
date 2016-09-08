#define MAIN_FILE
#include "Runner.h"

int Runner::compile_run(string filename,bool debug)
{  
   if (debug) {printf("Removing comments %s ...\n",filename.c_str());}
   FILE *infile = fopen (filename.c_str(), "rt");
   extern FILE * yyin;
   char outfilename[100];
   snprintf(outfilename,100,"%s%s",filename.c_str(),".strip");
   FILE* outfile = fopen (outfilename, "w");
   if (!infile) { printf("cannot open %s\n",filename.c_str()); return 1; }
   if (!outfile) { printf("cannot open %s\n",outfilename); return 1; }
   stripcmt(infile, outfile);
   fclose(infile);
   fclose(outfile);
   if (debug) {printf("Parsing... %s ...\n",filename.c_str());}
   extern FILE* yyin;
   yyin = fopen(outfilename,"r");
   glob = new ProgramNode();
   char errmsg[] = "error";
   char* ptr = (char*)errmsg;
   int result = yyparse();
   fclose(yyin);
   if(debug){glob->print(0);}
   CodeGenerator cg;
   cg.start(glob,NULL, debug);
   if (debug) {
      Disassembler d; 
      d.start(cg.getCodeBuffer(),cg.getHere(),NULL);}
   //
   // start interpreting
   //
   CInterpreter i(cg.getCodeBuffer(),NULL);
   i.start(debug);
}

