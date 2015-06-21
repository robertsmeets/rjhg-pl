#include "CompositeMethodCall.h"

#include <iostream>

using namespace std;

void CompositeMethodCall::addSingleMethodCall(SingleMethodCall*) {
}
;

void CompositeMethodCall::print(int level) {
	{for (unsigned int i = 0; i < level; i++) {
		cout << "+";
	}
	cout << "CompositeMethodCall" << endl;
	}
}
