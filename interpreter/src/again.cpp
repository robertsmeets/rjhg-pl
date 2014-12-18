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

#include "Interpreter.h"

using namespace std;

int main() {
	cout << "Start..." << endl;
	ifstream myfile("C:\\Users\\robert\\workspace\\again\\Debug\\example.bin", ios::binary);
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
	Interpreter i(buffer);
	i.start();
	return 0;
}


