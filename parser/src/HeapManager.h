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

class HeapManager {
   char* space;
   char* here;
   unsigned int size;
public:
	HeapManager();
	virtual ~HeapManager();
	char* allocate(unsigned int);
	char* getStart();
	unsigned short int getOffset();

};

#endif /* HEAPMANAGER_H_ */
