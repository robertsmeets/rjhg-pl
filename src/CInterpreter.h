/*
 * CInterpreter.h
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#ifndef INTERPRETER_SRC_INTERPRETER_H_
#define INTERPRETER_SRC_INTERPRETER_H_

#include <vector>
#include "stdio.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
//#include <dyncall.h>
//#include <dyncall_callf.h>
//#include <dynload.h>
#include <math.h>
#include "stdint.h"

#include "HeapManager.h"

#include "CFunctions.h"
#include "DebugInfo.h"

using namespace std;

class HeapManager; // forward declaration

struct stack_element {
      unsigned short int atype;
      unsigned short int address;
   };

class CInterpreter {
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
   vector<unsigned long> externs;
   uint16_t find_ext_proc_table(); 
   void check_magic_number();
   uint16_t find_offset();
public:
   CInterpreter(char*, DebugInfo*);
   virtual ~CInterpreter();
   void start();
   int execute_next();
   void print_a_string(char*,bool);
   void print_a_string(char*, uint16_t);
   void call_external(char*, uint16_t);
   vector<stack_element>* getStack();
};

#endif /* INTERPRETER_SRC_INTERPRETER_H_ */
