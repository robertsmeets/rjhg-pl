//============================================================================
// Name        : parser.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Parse.h"
#include "CodeGenerator.h"
#include "PException.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Must provide filename as an argument, example " << argv[0]
				<< " c:\\test\\test.src" << endl;
		return -1;
	}
	string filename = argv[1];
	cout << "Parsing... " << filename << " ... " << endl;
	Parse p;
	try {
		p.start(filename);
		CodeGenerator cg;
		cg.start(p.getPn());
	} catch (PException & E) {
		cout << "Exception: " << E.ShowReason() << endl;
		return -1;
	}
	return 0;
}
