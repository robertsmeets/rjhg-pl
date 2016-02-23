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

DCpointer Extern::address()
{
   string libpath = "msvcrt.dll";
   const char* lp = libpath.c_str();
   DLLib* ll = dlLoadLibrary(lp);
   if (ll == NULL) {
      printf("could not find library\n");
   }
   //
   // dlLoadLibrary loads a dynamic library at libpath and returns a handle
   // to it for use in dlFreeLibrary and dlFindSymbol calls.
   // dlFreeLibrary frees the loaded library with handle pLib.
   //
   DCpointer sym = dlFindSymbol(ll, name.c_str());
   if (sym == NULL) {
      puts("could not find external symbol");
   }
   return sym;
}
