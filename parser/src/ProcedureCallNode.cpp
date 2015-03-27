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
}

ProcedureCallNode::~ProcedureCallNode() {

}

string ProcedureCallNode::getProcedureName() {
	return procedure_name;
}

void ProcedureCallNode::setProcedureName(string a_procedure_name) {
	procedure_name = a_procedure_name;
}

void ProcedureCallNode::emit(CodeGenerator* cg) {
	//
	// put the parameters on the stack
	//
	for (vector<ExpressionNode>::iterator it = parameter_exps.begin();
			it != parameter_exps.end(); ++it) {
		cg->emitRpn((*it).getRpn(), pn, this);
	}
	cg->addCallTo(procedure_name, this);
}

void ProcedureCallNode::addParametersExpression(ExpressionNode en) {
	parameter_exps.push_back(en);
}

string ProcedureCallNode::stype() {
	return "procedure_call";
}
