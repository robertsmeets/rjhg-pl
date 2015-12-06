#include "If.h"

If::If(Expression* e,Statements* s1, Statements* s2)
{
    expression = e;
    statements1 = s1;
    statements2 = s2;
}

If::~If() {
}

void If::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "If " << endl;
	expression->print(level+1);
        statements1->print(level+1);
        if (statements2 != NULL)
        {
           statements2->print(level+1);
        }
}


void If::emit(CodeGenerator* cg)
{

}

