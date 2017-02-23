/*
 * CFunctions.cpp
 *
 *  Created on: Feb 25, 2015
 *      Author: robert
 */

#include "CFunctions.h"

using namespace std;

class CInterpreter; // forward declaration

unsigned int func_plus_ii(unsigned int i, unsigned int j) { return i + j; }
double func_plus_id(int i, double j) { return i + j; } 
double func_plus_di(double i, int j) { return i + j; }
double func_plus_dd(double i, double j) { return i + j; }
unsigned int func_minus_ii(unsigned int i, unsigned int j) { return i - j; }
double func_minus_id(int i, double j) { return i - j; }
double func_minus_di(double i, int j) { return i - j; }
double func_minus_dd(double i, double j) { return i - j; }
unsigned int func_mul_ii(unsigned int i, unsigned int j) { return i * j; }
double func_mul_id(int i, double j) { return i * j; }
double func_mul_di(double i, int j) { return i * j; }
double func_mul_dd(double i, double j) { return i * j; }
bool func_eq_ii(unsigned int i, unsigned int j) { return i == j; }
bool func_eq_id(int i, double j) { return i == j; }
bool func_eq_di(double i, int j) { return i == j; }
bool func_eq_dd(double i, double j) { return i == j; }
bool func_ne_ii(unsigned int i, unsigned int j) { return i != j; }
bool func_ne_id(unsigned int i, double j) { return i != j; }
bool func_ne_di(double i, unsigned int j) { return i != j; }
bool func_ne_dd(double i, double j) { return i != j; }
bool func_lt_ii(unsigned int i, unsigned int j) { return i < j; }
bool func_lt_id(unsigned int i, double j) { return i < j; }
bool func_lt_di(double i, int j) { return i < j; }
bool func_lt_dd(double i, double j) { return i < j; }
bool func_ge_ii(unsigned int i, unsigned int j) { return i >= j; }
bool func_ge_id(unsigned int i, double j) { return i >= j; }
bool func_ge_di(double i, unsigned int j) { return i >= j; }
bool func_ge_dd(double i, double j) { return i >= j; }
bool func_gt_ii(unsigned int i, unsigned int j) { return i > j; }
bool func_gt_id(unsigned int i, double j) { return i > j; }
bool func_gt_di(double i, unsigned int j) { return i > j; }
bool func_gt_dd(double i, double j) { return i > j; }
bool func_le_ii(unsigned int i, unsigned int j) { return i <= j; }
bool func_le_id(unsigned int i, double j) { return i <= j; }
bool func_le_di(double i, unsigned int j) { return i <= j; }
bool func_le_dd(double i, double j) { return i <= j; }
/**
 * this adds an element to an array 
 *
 */
void array_add(char* ptr,vector<stack_element>* s,uint16_t* t,bool debug,CInterpreter* i) 
{
   if (debug){printf("In the array_add\n");}
   //
   // byte 0 and 1 are the actual length
   // byte 2 and 3 are the claimed length
   //
   printf("ptr=%p\n",ptr);
   int actual = (*ptr & 0xff) + ((*(ptr + 1) & 0xff) << 8) ;
   if (debug)printf("actual = %d\n",actual);
   int claimed = (*(ptr+2) & 0xff)  + ((*(ptr + 3) & 0xff) << 8);
   if (debug)printf("actual = %d claimed = %d\n",actual,claimed);
   char* nptr = ptr+4;
   if (debug)printf("nptr = %p\n",nptr);
   char** ptrptr = (char**)nptr; 
   if (debug)printf("ptrptr = %p\n",ptrptr);
   char* spaceptr =  *ptrptr;
   if (debug)printf("spaceptr = %p\n",spaceptr);
   actual++;
   if (actual > claimed)
   {
      //
      // resize that array
      //
      claimed *= 2;
      if (debug) {printf("resize larger spaceptr = %p actual = %d claimed = %d\n",spaceptr,actual,claimed);};
      spaceptr = (char*)GC_REALLOC(spaceptr, 8 * claimed);
      if (debug) {printf("after larger\n");};
      *ptrptr = spaceptr;
   }
   if ((actual > 10) && (actual < claimed / 3))
   {
      //
      // resize that array
      //
      claimed /= 2;
      if (debug) {printf("resize smaller %p actual = %d claimed = %d\n",ptr,actual,8*claimed);};
      spaceptr = (char*)GC_REALLOC(spaceptr,8 * claimed);
      if (debug) {printf("after smaller\n");};
      *ptrptr = spaceptr;
   }
   //
   // set the new size
   //
   *ptr = actual & 0xff;
   *(ptr+1) = actual >> 8;
   //
   // set the new claimed value
   //
   *(ptr+2) = claimed & 0xff;
   *(ptr+3) = claimed >> 8;
   //
   // set the value
   //
   char* mptr = spaceptr + (actual-1) * 8;
   *mptr = (*s)[*t-2].atype & 0xff;
   char* avptr = (char*)(*s)[*t-2].address;
   char** vptr = (char**)(mptr+1);
   *vptr = avptr; 
   printf("--- ARRAY_ADD()\n");
   i->print_stack(); 
   (*t)--;
   i->print_stack(); 
}

void array_set(char* ptr,vector<stack_element>* s,uint16_t* t,bool debug,CInterpreter* i) 
{
   if (debug){printf("In the array_set\n");}
   //
   // byte 0 and 1 are the actual length
   // byte 2 and 3 are the claimed length
   //
   int actual = (*ptr & 0xff) + ((*(ptr + 1) & 0xff) << 8) ;
   int claimed = (*(ptr+2) & 0xff)  + ((*(ptr + 3) & 0xff) << 8);
   char* nptr = ptr+4;
   char** ptrptr = (char**)nptr; 
   char* spaceptr =  *ptrptr;
   if (debug)printf("actual = %d claimed = %d\n",actual,claimed);
   //
   // set the value
   //
   int atype = (*s)[*t-3].atype & 0xff;
   if (atype != TYPE_INT)
   {
       printf("array index is not an integer\n");
       exit(-1);
   }
   int index = (*s)[*t-3].address;
   if (index >= actual)
   {
       printf("array index %d out of range, array size is %d\n",index,actual);
       exit(-1);
   }       
   char* mptr = spaceptr + index * 8;
   *mptr = (*s)[*t-2].atype & 0xff;
   char* avptr = (char*)(*s)[*t-2].address;
   char** vptr = (char**)(mptr+1);
   *vptr = avptr; 
   printf("--- ARRAY_SET()\n");
   i->print_stack(); 
   (*t)--;
   (*t)--;
   i->print_stack(); 
}

/**
 *
 * String hash function
 *
 */
long stringhash(char* key) {
    long hashVal = 0;
    while (*key != '0') {
      hashVal = (hashVal << 4) + *(key++);
      long g = hashVal & 0xF0000000L;
      if (g != 0) hashVal ^= g >> 24;
      hashVal &= ~g;
   }
   return hashVal;
}      
