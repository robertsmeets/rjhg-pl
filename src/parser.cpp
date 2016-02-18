//============================================================================
// Name        : parser.cpp
//============================================================================

#include <iostream>

#include "Parse.h"
#include "CodeGenerator.h"
#include "PException.h"
#include "Disassembler.h"
#include "CInterpreter.h"
#include "DebugInfo.h"
#include "pProgramNode.h"


using namespace std;


int pmain(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Must provide filename as an argument, example " << argv[0]
				<< " c:\\\\test\\\\test.src" );
		return -1;
	}
	string filename = argv[1];
	printf("Parsing... " << filename << " ... " );
	Parse p;
	CodeGenerator cg;
	DebugInfo di(cg.getCodeBuffer(),p.getBuffer());
	try {
		//
		// start the parser
		//
		p.start(filename);


#ifdef DEBUG
		p.getPn()->print();
#endif
		cg.start(p.getPn(),&di);
#ifdef DEBUG
		Disassembler d;
		d.start(cg.getCodeBuffer(),cg.getHere(),&di);
#endif
		//
		// start interpreting
		//
		CInterpreter i(cg.getCodeBuffer(),&di);
		i.start();
	} catch (PException & E) {
		printf("Exception: " << E.ShowReason() );
		return -1;
	}
	return 0;
}
