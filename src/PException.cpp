/*
 * CException.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: robert
 */

#include "PException.h"

using namespace std;

PException::PException(string a_reason) {
	reason = a_reason;
}

PException::~PException() {
}

string PException::ShowReason() {
	return reason;
}

