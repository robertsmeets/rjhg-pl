/*
 * CInterpreter.h
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#ifndef INTERPRETER_SRC_INTERPRETER_H_
#define INTERPRETER_SRC_INTERPRETER_H_

#include <stdio.h>
#include <dyncall.h>
#include <dyncall_callf.h>
#include <dynload.h>
#include <math.h>
#include <stdint.h>
#include <gc.h>
#include <stdbool.h>

#include "CConstants.h"
#include "jwHash.h"
#include "CFunctions.h"

struct stack_element {
      unsigned short int atype;
      unsigned long long int address;
};

struct extern_record {
   unsigned long long int address;
   char* signature;
};

   bool debug;
   char* buffer;
   uint16_t pc;
   uint16_t t;
   uint16_t tr;
   uint16_t tb;
   struct stack_element s[500];
   unsigned short int r[500];
   unsigned short int b[500];
   // HeapManager* hm;

   //typedef void (*especial) (char* ,vector<stack_element>* ,uint16_t* ,bool,CInterpreter* );
   typedef uint16_t (*iiptr)(uint16_t, uint16_t);
   typedef double (*ddptr)(double, double);
   typedef double (*idptr)(uint16_t, double);
   typedef double (*diptr)(double, uint16_t);

   typedef bool (*biiptr)(uint16_t, uint16_t);
   typedef bool (*bddptr)(double, double);
   typedef bool (*bidptr)(uint16_t, double);
   typedef bool (*bdiptr)(double, uint16_t);
   iiptr fptrs[14][8][8];

   jwHashTable* methodmap;
   unsigned int extern_count = 0;
   struct extern_record externs[100];
   uint16_t find_ext_proc_table(); 
   void check_magic_number();
   uint16_t find_offset();
   void call_external(short unsigned int,short unsigned int) ;
   void pass_in_arg( DCCallVM* ,char ,struct stack_element);
   void start(bool);
   int execute_next();
   void print_a_string(char*,bool);
   void print_stack();
   struct stack_element* getStack();
   int getStackDepth();

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
