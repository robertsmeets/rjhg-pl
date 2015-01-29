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
#include "Disassembler.h"

class Disassembler;
// forward declaration

using namespace std;

int interpreter_main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Must provide filename as an argument, example " << argv[0]
				<< " c:\\test\\test.bin" << endl;
		;
		return -1;
	}
	string filename = argv[1];
	cout << "Start... " << filename << " ... " << endl;
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
	if (!myfile.read(buffer.data(), size)) {
		cout << "something went wrong" << endl;
		return -1;
	}

	//
	// buffer now contains it
	//
	myfile.close();
	//Disassembler d;
	//d.start(buffer);
	//
	// start interpreting
	//
	//CInterpreter i(buffer);
	//i.start();
	return 0;
}

