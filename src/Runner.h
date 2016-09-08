#ifndef RUNNER_H_
#define RUNNER_H_ 

#include "Comments.h"
#include "CodeGenerator.h"
#include "Disassembler.h"
#include "CInterpreter.h"
#include "stdio.h"

using namespace std;
extern int yyparse();


#ifdef  MAIN_FILE
ProgramNode* glob;
#else
extern ProgramNode* glob;
#endif


class Runner {

public:
   int compile_run(string,bool);
};
#endif
