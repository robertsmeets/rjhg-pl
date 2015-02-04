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
	size = 256;
	space = (char*) malloc(size);
	here = space;
}

HeapManager::~HeapManager() {

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

void HeapManager::deallocate(char* ptr) {

}
