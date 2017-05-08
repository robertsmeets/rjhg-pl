#ifndef RUNNER_H_
#define RUNNER_H_ 

#include "Comments.h"
#include "CodeGenerator.h"
#include "Disassembler.h"
#include "CInterpreter.h"
#include "stdio.h"
#include "Assignment.h"
#include "If.h"
#include "While.h"
#include "PrintNode.h"
#include "Literal.h"
#include "ProcedureCall.h"
#include "MethodCall.h"
//#include "y.tab.h"

using namespace std;
 int yyparse();


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
