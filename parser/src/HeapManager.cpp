/*
 * HeapManager.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: robert
 */

#include "HeapManager.h"

HeapManager::HeapManager() {
	//
	// initial allocation
	//
<<<<<<< HEAD
	size = 2560;
=======
	size = 256;
>>>>>>> Float, String literals now work
	space = (char*) malloc(size);
	here = space;
}

HeapManager::~HeapManager() {
<<<<<<< HEAD
	free(space);
=======

>>>>>>> Float, String literals now work
}

char* HeapManager::allocate(unsigned int nbytes) {
	unsigned int available = here - space;
	char* ptr;
	if (available < nbytes) {
		size *= 2;
		space = (char*)realloc(space, size);
		here = space + available;
	}
	ptr = here;
	here += nbytes;
	return ptr;
}

<<<<<<< HEAD
char* HeapManager::getStart()
{
	return space;
}

unsigned short int HeapManager::getOffset()
{
	return here - space;
=======
void HeapManager::deallocate(char* ptr) {

>>>>>>> Float, String literals now work
}
