#ifndef FILECREATOR_H_
#define FILECREATOR_H_ 

#include <iostream>
#ifndef _WIN32
#include <sys/stat.h>
#endif 
#include "stdio.h"
#include <string>

using namespace std;

class FileCreator {

public:
   void start(char*, char* , int );
};
#endif


