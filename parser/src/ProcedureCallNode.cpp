/*
 * ProcedureCallNode.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: robert
 */

#include "ProcedureCallNode.h"

using namespace std;

ProcedureCallNode::ProcedureCallNode(ProcedureNode* p, unsigned int linep,
		unsigned int charp, unsigned int absp) {
	linepos = linep;
	charpos = charp;
	pn = p;
	abspos = absp;
	parameter_exps = vector<ExpressionNode>();
	method = false;
}

ProcedureCallNode::~ProcedureCallNode() {

}

string ProcedureCallNode::getProcedureName() {
	return procedure_name;
}

void ProcedureCallNode::setProcedureName(string a_procedure_name) {
	procedure_name = a_procedure_name;
}

void ProcedureCallNode::setLhsExpression(ExpressionNode en) {
	LhsExpression = en;
}

void ProcedureCallNode::setMethod(bool b) {
	method = b;
}

void ProcedureCallNode::emit(CodeGenerator* cg) {
	//
	// put the parameters on the stack
	//
	for (vector<ExpressionNode>::iterator it = parameter_exps.begin();
			it != parameter_exps.end(); ++it) {
		cg->emitRpn((*it).getRpn(), pn, this);
	}
	if (method) {
		cg->emitRpn(LhsExpression.getRpn(), pn, this);
		cg->addCallToMethod(procedure_name, this);
	} else {
		cout << "ProcedureCallNode::emit(" << procedure_name << endl;
		cg->addCallToProc(procedure_name, this);
	}
}

void ProcedureCallNode::addParametersExpression(ExpressionNode en) {
	parameter_exps.push_back(en);
}

string ProcedureCallNode::stype() {
	return "procedure_call";
}

void ProcedureCallNode::print( int level) {
	for ( int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "ProcedureCallNode ";
	if (method) {
		cout << "method " << procedure_name;
	} else {
		cout << "proc " << procedure_name;
	}
	cout << endl;
	cout << " LHS = " << endl;
	LhsExpression.print(level + 1);
	cout << " RHS = " << endl;
	for (auto pe : parameter_exps) {
		pe.print(level + 1);
	}
}
