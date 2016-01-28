/*
 * DebugInfo.cpp
 *
 *  Created on: Mar 10, 2015
 *      Author: robert
 */

#include "DebugInfo.h"

using namespace std;

DebugInfo::DebugInfo(char* cb, vector<char, allocator<char> >*a_textbuffer) {
	lastprinted = 0;
	buffer = cb;
	textbuffer = a_textbuffer;
}

DebugInfo::~DebugInfo() {
}

void DebugInfo::setPosition(unsigned int here, unsigned int linep,
		unsigned int charp, unsigned int absp) {
	cposition my_position;
	my_position.code_location = here;
	my_position.linepos = linep;
	my_position.charpos = charp;
	my_position.abspos = absp;
	code_location.push_back(my_position);
}

void DebugInfo::reset() {
	lastprinted = 0;
}

/**
 * i is the compiled code location
 */
void DebugInfo::printLine(unsigned int i) {
	unsigned int found = 0;
	//
	// find the text location of i
	//
	for (auto it = code_location.begin(); it < code_location.end(); it++) {
		if ((*it).code_location == i) {
			found = (*it).abspos;
			break;
		}
	}
	//
	// found is the abs location in the text
	//
	if ((found != 0) && (lastprinted != found)) {
		cout << "==> " << hex << i << dec << ": ";
		for (unsigned int j = found;; j++) {
			char c = (*textbuffer)[j];
			if (c == '\0' || c == '\n' || c == '\r') {
				break;
			}
			cout << c;
		}
		cout << endl;
		lastprinted = found;
	}
}

