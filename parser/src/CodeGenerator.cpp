/*
 * CodeGenerator.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include <iostream>

#include "CodeGenerator.h"
#include "ProgramNode.h"

using namespace std;

CodeGenerator::CodeGenerator() {


}

CodeGenerator::~CodeGenerator() {

}

void CodeGenerator::start(ProgramNode pn)
{
    cout << "Generating code..." << endl;
	vector<ProcedureNode> procs;
    procs = pn.getProcedures();
    //
    // only generate code for the main procedure
    //
    cout << "there are " << procs.size() << " procs" << endl;
    for( vector<ProcedureNode>::iterator it = procs.begin(); it != procs.end(); ++it) {
    	    ProcedureNode a_proc =  *it;
    	    string pname = a_proc.getName();
    	    if (pname == "main")
    	    {
    	    	cout << "found main" << endl;
                 start_proc(a_proc);
    	    }
    	    else
    	    {
    	    	cout << "found something else: <"<< pname << ">" << endl;

    	    }
    	}

}

void CodeGenerator::start_proc(ProcedureNode a_proc)
{
	vector<AssignmentNode> assignments = a_proc.getAssignments();
	for( vector<AssignmentNode>::iterator it = assignments.begin(); it != assignments.end(); ++it) {
cout << "assignment" << endl;
	}

}
