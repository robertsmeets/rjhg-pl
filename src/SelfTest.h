#ifndef SELFTEST_H_
#define SELFTEST_H_ 
//#define YYSTYPE

#include "Runner.h"
#include "stdio.h"
#include <string>

using namespace std;

class SelfTest {

public:
   void run();
private:
   void Test(char*);
   bool Compare(char*,char*);
};
#endif
