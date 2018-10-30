/*
 * CInterpreter.h
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#ifndef INTERPRETER_SRC_INTERPRETER_H_
#define INTERPRETER_SRC_INTERPRETER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <gc.h>
#include <stdbool.h>
#include <ffi.h>
#include <dlfcn.h>
#include <inttypes.h>

// do not mix this with   typedef void* (*arbitrary)(void); 
typedef void* (*arbitrary)();

#include "CConstants.h"
#include "jwHash.h"
#include "CFunctions.h"

struct stack_element {
      unsigned short int atype;
      unsigned long long int address;
};

struct extern_record {
   char* signature;
   char* name;
};

   static bool debug;
   static char* buffer;
   static uint16_t pc;
   static uint16_t t;
   static uint16_t tr;
   static uint16_t tb;
   static struct stack_element s[500];
   static unsigned short int r[500];
   static unsigned short int b[500];

   typedef long (*iiptr)(long, long);
   typedef double (*ddptr)(double, double);
   typedef double (*idptr)(uint16_t, double);
   typedef double (*diptr)(double, uint16_t);

   typedef bool (*biiptr)(uint16_t, uint16_t);
   typedef bool (*bddptr)(double, double);
   typedef bool (*bidptr)(uint16_t, double);
   typedef bool (*bdiptr)(double, uint16_t);
   static iiptr fptrs[35][8][8];

   static jwHashTable* methodmap;
   static unsigned int extern_count ;
   static struct extern_record externs[100];
   int CI_getStackDepth();
   struct stack_element* CI_getStack();
   void CI_check_magic_number();
   uint16_t CI_find_offset();
   uint16_t CI_find_ext_proc_table();
   int CI_execute_next();
   void CI_print_stack();
   void CI_print_a_string(char* ,bool ); 
   void CI_print_a_string2(char* , uint16_t ); 
   void CI_call_external(short unsigned int ,short unsigned int ); 
   ffi_type* CI_value(char, struct stack_element);
   void* CI_pass_in_arg(char ,struct stack_element );
   ffi_type* CI_outgoing(char );
   void* CI_find_ext_address(char*);

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
