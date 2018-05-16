#include "Extern.h"

using namespace std;

Extern::Extern(string a_name,string an_estring) {
   name = a_name;
   estring = an_estring;
}

Extern::~Extern() {
}

void Extern::print(int level) {
   for (int i = 0; i < level; i++) {
      printf("+");
   } 
   printf("Extern \n" );
}

void Extern::setNumber(int num)
{
   number = num;
}

int Extern::getNumber()
{
   return number;
}

string Extern::getName()
{
   return name;
}

string Extern::getEstring()
{
   return estring;
}

