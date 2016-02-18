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
	size = 4096;
	space = (char*) malloc(size);
	here = space;
	interpreter = NULL;
}

HeapManager::~HeapManager() {
	free(space);
}

void HeapManager::setInterpreter(CInterpreter* ci) {
	interpreter = ci;
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
		garbageCollect();
	}
	used = here - space;
	available = size - used;
	if (available < nbytes) {
		size = max(2 * size, used + 2 * nbytes);
		space = (char*) realloc(space, size);
		here = space + used;
	}
	ptr = here;
	here += nbytes;
	return ptr;
}

char* HeapManager::getStart() {
	return space;
}

unsigned short int HeapManager::getOffset() {
	return here - space;
}
struct saddress {
	unsigned int address;
	unsigned int len;

};
bool acompare(saddress lhs, saddress rhs) {
	return lhs.address < rhs.address;
}
void HeapManager::garbageCollect() {
	//
	// loop over the stack
	//
	map<unsigned int, unsigned int> addresses;   // relative address, length
	map<unsigned int, unsigned int> references; // rel. address, stack index
	map<unsigned int, unsigned int> movetable; // old rel. address, new rel.address
	vector<stack_element>* s = interpreter->getStack();
	unsigned int i = 0;
	for (auto const &an_element : *s) {
		//
		// the following types can be found
		//
		//   0: Null
		//	 2: integer
		//	 5: float            -> stored on the heap
		//   6: boolean
		//	 7: string           -> stored on the heap
		//
		unsigned int t = an_element.atype;
		if (t == 5) { // float
			//
			// the size is 8
			//
			unsigned int address = an_element.address;
			unsigned int len = 8;
			//
			// store the address reference
			//
			addresses[address] = len;
			references[address] = i;

		} else if (t == 7) { // string
			//
			// figure out the size
			//
			unsigned int address = an_element.address;
			char* ptr = space + address;
			unsigned int len = ((*ptr) & 255) + (*(ptr + 1) >> 8) + 2;
			//
			// store the address reference
			//
			addresses[address] = len;
			references[address] = i;
		} else if (t == 8) { // object
			unsigned int address = an_element.address;
			char* ptr = space + address;
			unsigned int len = (*(ptr + 2) & 255) * 2 + 3;
			//
			// store the address reference
			//
			addresses[address] = len;
			references[address] = i;

		}
		i++;
	}
	//
	// addresses must be sorted now
	// transfer them to a vector
	//
	vector<saddress> vaddresses;
	for (auto const &it : addresses) {
		//
		// it.first is the address
		// it.second is the length
		//
		saddress s;
		s.address = it.first;
		s.len = it.second;
		vaddresses.push_back(s);
	}
	//
	// sort them
	//
	sort(vaddresses.begin(), vaddresses.end(), acompare);
	//
	// the references are gathered. Find the holes.
	//
#ifdef DEBUG
	printf("--- Here come the addresses" );
#endif
	unsigned int last = 0;
	for (auto const &it : vaddresses) {
		//
		// it.first is the address
		// it.second is the length
		//
#ifdef DEBUG
		printf("address " << it.address << " length " << it.len );
#endif
		if (it.address > last) {
			//
			// there is a hole
			//
			// Move the data. Use memmove since the areas may overlap.
			// memmove(destination,origin,length)
			//
			memmove(space + last, space + it.address, it.len);
#ifdef DEBUG
			printf("Moved " << it.len << " bytes from " << it.address << " to " << last );
#endif
			movetable[it.address] = last;
		}
		last += it.len;
	}
	//
	// set here to the new value
	//
#ifdef DEBUG
	printf("reclaimed " << here - space - last << " bytes" );
#endif
	here = space + last;
#ifdef DEBUG
	printf("--- Those were the addresses" );
#endif
	//
	// all data is moved. Now fix up the addresses.
	// loop over all references
	//
	for (auto const &it : references) {
		if (movetable.find(it.first) != movetable.end()) {
			unsigned int new_address = movetable[it.first];
#ifdef DEBUG
			printf("fixing stack entry "<< (*s)[it.second].address <<" to " << new_address );
#endif
			(*s)[it.second].address = new_address;
		}
	}

}
