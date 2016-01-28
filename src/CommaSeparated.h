#ifndef COMMASEPARATED_H_
#define COMMASEPARATED_H_ 

#include <string> 
#include <vector>

using namespace std;

class CommaSeparated {
private:
   vector<string>* identifiers;
public:
   CommaSeparated();
   void addIdentifier(char*);
   void print(int);
   vector<string>* getIdentifiers();
};
#endif
