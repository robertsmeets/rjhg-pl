#include "CompositeMethodCall.h"

#include <iostream>

using namespace std;

void CompositeMethodCall::addSingleMethodCall(SingleMethodCall*) {
}
;

void CompositeMethodCall::print(int i) {
	cout << "CompositeMethodCall" << endl;
}
