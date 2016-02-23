#ifndef EXTERN_H_ 
#define EXTERN_H_ 

#include <dyncall.h>
#include <dyncall_callf.h>
#include <dynload.h>
#include <string>
#include "stdio.h"
#include <vector>

using namespace std;

class Extern {
private:
   string name;
   string estring;
   int number;   
public:
   Extern(string,string);
   virtual ~Extern();
   void print(int);
   string getEstring();
   string getName();
   void setNumber(int);
   int getNumber();
   DCpointer address();
};

#endif
