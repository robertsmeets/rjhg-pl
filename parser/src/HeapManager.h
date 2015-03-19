/*
 * HeapManager.h
 *
 *  Created on: Feb 4, 2015
 *      Author: robert
 */

#ifndef HEAPMANAGER_H_
#define HEAPMANAGER_H_

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <map>
#include <cstring>

#include "CInterpreter.h"

class CInterpreter; // forward declaration

class HeapManager {
	char* space;        // absolute address, beginning of heap
	char* here; 		// absolute address, beginning of free space
	unsigned int size; // size of the heap incl. unused space
	CInterpreter* interpreter;

public:
	HeapManager();
	virtual ~HeapManager();
	char* allocate(unsigned int);
	char* getStart();
	unsigned short int getOffset();
	void garbageCollect();
	void setInterpreter(CInterpreter*);
};

#endif /* HEAPMANAGER_H_ */
