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
#include "AstPrinter.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Must provide filename as an argument, example " << argv[0]
				<< " c:\\\\test\\\\test.src" << endl;
		return -1;
	}
	string filename = argv[1];
	cout << "Parsing... " << filename << " ... " << endl;
	Parse p;
	CodeGenerator cg;
	AstPrinter ap;
	DebugInfo di(cg.getCodeBuffer(),p.getBuffer());
	try {
		p.start(filename);
		ap.start(p.getPn());
		cg.start(p.getPn(),&di);

		Disassembler d;
		d = Disassembler();
		d.start(cg.getCodeBuffer(),cg.getHere(),&di);
		//
		// start interpreting
		//
		CInterpreter i(cg.getCodeBuffer(),&di);
		i.start();
	} catch (PException & E) {
		cout << "Exception: " << E.ShowReason() << endl;
		return -1;
	}
	return 0;
}
