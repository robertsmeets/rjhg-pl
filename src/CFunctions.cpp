/*
 * CFunctions.cpp
 *
 *  Created on: Feb 25, 2015
 *      Author: robert
 */

#include "CFunctions.h"

using namespace std;

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
void array_add(char* ptr,vector<stack_element>* s,uint16_t* t,bool debug) 
{
   if (debug){printf("In the array_add\n");}
   //
   // byte 0 and 1 are the actual length
   // byte 2 and 3 are the claimed length
   //
   int actual = (*ptr & 0xff) + ((*(ptr + 1) & 0xff) << 8) ;
   int claimed = (*(ptr+2) & 0xff)  + ((*(ptr + 3) & 0xff) << 8);
   actual++;
   if (actual > claimed)
   {
      //
      // resize that array
      //
      claimed *= 2;
      ptr = (char*)GC_realloc(ptr, 8 * claimed + 4);
      (*s)[*t-1].address = (long long unsigned int)ptr;
   }
   if ((actual > 10) && (actual < claimed / 3))
   {
      //
      // resize that array
      //
      claimed /= 2;
      ptr = (char*)GC_realloc(ptr,8 * claimed + 4);
      vector<stack_element> thestack = *s;
      (*s)[*t-1].address = (long long unsigned int)ptr;
   }
   //
   // set the value
   //
   char* nptr = ptr + (actual - 1) * 8 + 4;
   *nptr = (*s)[*t].atype;
   unsigned long long int a = (*s)[*t].address;
   *(nptr+1) = a & 0xff;
   *(nptr+2) = (a >> 8) & 0xff;
   *(nptr+3) = (a >> 16) & 0xff;
   *(nptr+4) = (a >> 24) & 0xff;
   (*t)--;
   (*t)--;
}

