/*
 * Disassembler.h
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include <sstream>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "DebugInfo.h"
#include "CConstants.h"

using namespace std;

class Disassembler {
   DebugInfo* di;
   uint16_t i;
   char* buffer;
public:
   Disassembler();
   virtual ~Disassembler();
   void start(char*, unsigned int, DebugInfo*);
   string decode(char,uint16_t, uint16_t);
   static void hexdump(char*, unsigned int);
   void print_tables();
   void print_magic_number();
   uint16_t find_offset(); 
   uint16_t find_ext_proc_table(); 
};

#endif /* DISASSEMBLER_H_ */

