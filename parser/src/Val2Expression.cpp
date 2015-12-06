#include "Val2Expression.h"

Val2Expression::Val2Expression(char c, Expression* lhs_e, Expression* rhs_e) {
	operation = c;
	lhs = lhs_e;
	rhs = rhs_e;
}

void Val2Expression::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Val2Expression op=[" << operation << "]" << endl;
	lhs->print(level + 1);
	rhs->print(level + 1);
}


void Val2Expression::emit(CodeGenerator* cg)
{

}
