//============================================================================
// Name        : again.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>

#include "CInterpreter.h"

using namespace std;

int main() {
	cout << "Start..." << endl;
	string filename = "F:\\robert\\projects\\parser\\code\\example.bin";
	ifstream myfile(filename.c_str(), ios::binary);
	streampos begin, end;
	begin = myfile.tellg();
	myfile.seekg(0, ios::end);
	end = myfile.tellg();
	cout << "size is: " << (end - begin) << " bytes.\n";
	int size = end - begin;
	//
	// reading it into a buffer
	//
	vector<char> buffer(size);
	myfile.clear();
	myfile.seekg(0, ios::beg);
	if (myfile.read(buffer.data(), size))
	{
	    /* worked! */
	}
	else
	{
		cout << "something went wrong" << endl;
	}

	//
	// buffer now contains it
	//
	myfile.close();

	//
	// start interpreting
	//
	CInterpreter i(buffer);
	i.start();
	return 0;
}


