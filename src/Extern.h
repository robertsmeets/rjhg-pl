#ifndef EXTERN_H_ 
#define EXTERN_H_ 

#include "stdio.h"
#include "gc_cpp.h"
#include <string>
#include <vector>

using namespace std;

class Extern:public gc {
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
};

#endif
