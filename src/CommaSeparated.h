#ifndef COMMASEPARATED_H_
#define COMMASEPARATED_H_ 

#include "gc_cpp.h"
#include <string> 
#include <vector>

using namespace std;

class CommaSeparated:public gc {
private:
   vector<string>* identifiers;
public:
   CommaSeparated();
   void addIdentifier(char*);
   void print(int);
   vector<string>* getIdentifiers();
};
#endif
