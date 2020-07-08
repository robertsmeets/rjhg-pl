/*
 * CFunctions.cpp
 *
 *  Created on: Feb 25, 2015
 *      Author: robert
 */

#include "CFunctions.h"

long func_plus_ii(long i, long j) { return i + j; }
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
bool func_eq_nn(unsigned int i, unsigned int j) { return true ; }
bool func_eq_ii(unsigned int i, unsigned int j) { return i == j; }
bool func_eq_id(int i, double j) { return i == j; }
bool func_eq_di(double i, int j) { return i == j; }
bool func_eq_dd(double i, double j) { return i == j; }
bool func_eq_on(unsigned int i, unsigned int j) { return false; }
bool func_eq_no(unsigned int i, unsigned int j) { return false; }
bool func_ne_nn(unsigned int i, unsigned int j) { return false ; }
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
 *
 * return the size of a string
 *
 */
void string_size(char* ptr,struct stack_element* s,uint16_t* t,bool debug) 
{
   struct stack_element fr1 = s[*t-1];
   if (fr1.atype != TYPE_STRING)
   {
      printf("size requested, but this is not a string\n");
      exit(-1);
   }
   char * ptr1 = (char*) fr1.address;
   uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
   s[*t].atype=TYPE_INT;
   s[*t].address=len1;
   (*t)++; 
}


/**
 *
 * this adds an element to an array 
 *
 */
void array_add(char* ptr,struct stack_element* s,uint16_t* t,bool debug) 
{
   //
   // byte 0 and 1 are the actual length
   // byte 2 and 3 are the claimed length
   //
   uint16_t* uptr = (uint16_t*) ptr;
   uint16_t actual = *uptr ;
   uint16_t* cptr = (uint16_t*) (ptr+2);
   uint16_t claimed = *cptr;
   char* nptr = ptr+4;
   char** ptrptr = (char**)nptr; 
   char* spaceptr =  *ptrptr;
   actual++;
   if (actual > claimed)
   {
      //
      // resize that array
      //
      spaceptr = (char*)GC_MALLOC(18 * claimed);
      memcpy(spaceptr,*ptrptr,9*claimed);
      claimed *= 2;
      *ptrptr = spaceptr;
      if (debug) {printf("after larger spaceptr = %p actual = %d claimed = %d\n",spaceptr,actual,claimed);};
   }
   if ((actual > 10) && (actual < claimed / 3))
   {
      //
      // resize that array
      //
      claimed /= 2;
      spaceptr = (char*)GC_MALLOC(9 * claimed);
      if (debug) printf("copying %d bytes from %p to %p\n",claimed,*ptrptr,spaceptr);
      memcpy(spaceptr,*ptrptr,9*claimed);
      if (debug) {printf("resize smaller %p actual = %d claimed = %d\n",ptr,actual,9*claimed);};
      *ptrptr = spaceptr;
      if (debug) {printf("after smaller\n");};
   }
   //
   // set the new size
   //
   *uptr = actual;
   //
   // set the new claimed value
   //
   *cptr = claimed;
   //
   // set the value
   //
   //
   // mptr is the location of the new value
   //
   char* mptr = spaceptr + (actual-1) * 9;
   //
   // place the type in the location
   //
   *mptr = s[*t-2].atype & 0xff;
   //
   // avptr is the value
   //
   char* avptr = (char*)s[*t-2].address;
   //
   // vptr is the location where the value should be placed
   //
   char** vptr = (char**)(mptr+1);
   //
   // copy the value
   //
   memcpy(vptr,&avptr,8);
   (*t)--;
   (*t)--;
}

void array_set(char* ptr,struct stack_element* s,uint16_t* t,bool debug) 
{
   //
   // byte 0 and 1 are the actual length
   // byte 2 and 3 are the claimed length
   //
   int actual = (*ptr & 0xff) + ((*(ptr + 1) & 0xff) << 8) ;
   int claimed = (*(ptr+2) & 0xff)  + ((*(ptr + 3) & 0xff) << 8);
   char* nptr = ptr+4;
   char** ptrptr = (char**)nptr; 
   char* spaceptr =  *ptrptr;
   //
   // set the value
   //
   int atype = s[*t-3].atype & 0xff;
   if (atype != TYPE_INT)
   {
       printf("array index is not an integer\n");
       exit(-1);
   }
   int index = s[*t-3].address;
   if (index >= actual)
   {
       printf("array index %d out of range, array size is %d\n",index,actual);
       exit(-1);
   }       
   char* mptr = spaceptr + index * 9;
   *mptr = s[*t-2].atype & 0xff;
   char* avptr = (char*)s[*t-2].address;
   char** vptr = (char**)(mptr+1);
   *vptr = avptr; 
   (*t)--;
   (*t)--;
}

void crun(char* ptr,struct stack_element* s,uint16_t* t,bool debug)
{
   if(debug)printf("------------------ crun()\n");
   struct stack_element fr1 = s[*t-1];
   if (fr1.atype != TYPE_STRING)
   {
      printf("Not a string but a <%d>\n",fr1.atype);
      exit(-1);
   }
    char* cptr = (char*) fr1.address;
    uint16_t* ptr1 = (uint16_t*) cptr;
    uint16_t len1 = *ptr1;
    if(debug)printf("len = %d\n",len1);
    char* ad = cptr + 2;
    char* stringy = malloc(len1+1);
    strncpy(stringy,ad,len1);
    stringy[len1] = '\0';
    if(debug)printf("Found string <%s>\n",stringy);
    TCCState *astate;
    astate = tcc_new();
    if (!astate) {
        printf("Could not create tcc state\n");
        exit(-1);
    }
    tcc_set_lib_path(astate,"../tcc/lib");
    tcc_add_include_path(astate,"../tcc/include");
    tcc_add_library_path(astate,"../tcc/lib");
    tcc_set_output_type(astate, TCC_OUTPUT_MEMORY);
    if (tcc_compile_string(astate, stringy) == -1)
    {
       printf("Could not compile\n");
       exit(-1);
    }
    if (tcc_relocate(astate, TCC_RELOCATE_AUTO) < 0) { printf("could not relocate\n"); exit(-1); }
    int (*func)(int);
    func = tcc_get_symbol(astate, "tccmain");
    if (!func)
    {
      printf("could not find tccmain()\n");
      exit(-1);
    }
    /* run the code */
    if(debug)printf("------- before running the code\n");
    int result = func(0);
    if(debug)printf("------- after running the code result = %d\n",result);
    tcc_delete(astate);
}
