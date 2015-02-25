/*
 * HeapManager.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: robert
 */

#include "HeapManager.h"

using namespace std;

HeapManager::HeapManager() {
	//
	// initial allocation
	//
	size = 8092;
	space = (char*) malloc(size);
	here = space;
}

HeapManager::~HeapManager() {
	free(space);
}

//
// allocate nbytes of memory and return a pointer to it
//
// "here" is the pointer to the current free space
// "space" is the start of memory, it may be moved around
//
//
char* HeapManager::allocate(unsigned int nbytes) {
	unsigned int used = here - space;
	unsigned int available = size - used;
	char* ptr;
	if (available < nbytes) {
		size = max(2 * size, used+ 2 * nbytes);
#ifdef DEBUG
		cout << "--- Realloc " << size << endl;
#endif
		space = (char*)realloc(space, size);
		here = space + used;
	}
	ptr = here;
	here += nbytes;
	return ptr;
}

char* HeapManager::getStart()
{
	return space;
}

unsigned short int HeapManager::getOffset()
{
	return here - space;
}
