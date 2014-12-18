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

using namespace std;

int main() {
	cout << "Parsing..." << endl;
	Parse p;
    p.start();
    CodeGenerator cg;
    cg.start(p.getPn());
    return 0;
}
