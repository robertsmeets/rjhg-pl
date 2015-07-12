#include "Assignment.h"

Assignment::~Assignment()
{}

void Assignment::print(int level) {
	for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "Assignment" << endl;
}
