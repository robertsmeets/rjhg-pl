/*
 * CInterpreter.h
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#ifndef INTERPRETER_SRC_INTERPRETER_H_
#define INTERPRETER_SRC_INTERPRETER_H_

#include <vector>
#include <map>
#include "stdio.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <dyncall.h>
#include <dyncall_callf.h>
#include <dynload.h>
#include <math.h>
#include "stdint.h"
#include <gc.h>

#include "Disassembler.h"
#include "CConstants.h"
#include "DebugInfo.h"

using namespace std;

class HeapManager; // forward declaration

struct stack_element {
      unsigned short int atype;
      unsigned long long int address;
};
#include "CFunctions.h"

struct extern_record {
   unsigned long long int address;
   string signature;
};

class CInterpreter {
   bool debug;
   DebugInfo* di;
   char* buffer;
   uint16_t pc;
   uint16_t t;
   uint16_t tr;
   uint16_t tb;
   vector<stack_element> s;
   vector<unsigned short int> r;
   vector<unsigned short int> b;
   HeapManager* hm;

   typedef void (*especial) (char* ,vector<stack_element>* ,uint16_t* ,bool,CInterpreter* );
   typedef uint16_t (*iiptr)(uint16_t, uint16_t);
   typedef double (*ddptr)(double, double);
   typedef double (*idptr)(uint16_t, double);
   typedef double (*diptr)(double, uint16_t);

   typedef bool (*biiptr)(uint16_t, uint16_t);
   typedef bool (*bddptr)(double, double);
   typedef bool (*bidptr)(uint16_t, double);
   typedef bool (*bdiptr)(double, uint16_t);
   iiptr fptrs[14][8][8];

   map<uint16_t,map<uint16_t, uint16_t[3]>> methodmap;
   vector<extern_record> externs;
   uint16_t find_ext_proc_table(); 
   void check_magic_number();
   uint16_t find_offset();
   void call_external(short unsigned int,short unsigned int) ;
   void pass_in_arg( DCCallVM* ,char ,stack_element);
public:
   CInterpreter(char*, DebugInfo*);
   virtual ~CInterpreter();
   void start(bool);
   int execute_next(bool);
   void print_a_string(char*,bool);
   void print_a_string(char*, uint16_t);
   void print_stack();
   vector<stack_element>* getStack();
   int getStackDepth();
};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
