/*
 * DebugInfo.h
 *
 *  Created on: Mar 10, 2015
 *      Author: robert
 */

#ifndef DEBUGINFO_H_
#define DEBUGINFO_H_

#include <vector>
#include "stdio.h"

using namespace std;

class DebugInfo {
	struct cposition {
		unsigned int code_location;
		unsigned int linepos;
		unsigned int charpos;
		unsigned int abspos;
	};
	vector<cposition> code_location;
	unsigned int lastprinted;
	char* buffer;
	vector<char, allocator<char> >* textbuffer;
public:
	DebugInfo(char*,vector<char, allocator<char> >* );
	virtual ~DebugInfo();
	void setPosition(unsigned int, unsigned int, unsigned int, unsigned int);
	void reset();
	void printLine(unsigned int);
};

#endif /* DEBUGINFO_H_ */
