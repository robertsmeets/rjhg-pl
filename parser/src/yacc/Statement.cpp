#include "Statement.h"

Statement::~Statement()
{

}

void Statement::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Statement" << endl;
}

