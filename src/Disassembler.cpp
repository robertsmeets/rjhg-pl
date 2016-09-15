/*
 * Disassembler.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

#include <iostream>
#include <sstream>

#include "Disassembler.h"

using namespace std;

Disassembler::Disassembler() {
   di = NULL;
   i = 0;
}

Disassembler::~Disassembler() {
}

void Disassembler::start(char* inbuffer, unsigned int size, DebugInfo* a_di) {
   printf("Starting disassembler...\n" );
   buffer = inbuffer;
   if (!(buffer[0] == 'R' && buffer[1] == 'J' && buffer[2] == 'H'
         && buffer[3] == 'G' && buffer[4] == 'P' && buffer[5] == 'L')) {
      puts("Magic number does not match, invalid bytecode");
   }
   //
   // hexdump
   //
   hexdump(buffer, size);
   print_tables();
   i = buffer[6] + buffer[7] * 256;
   for (; i < size;) {
      printf("%04X ",i );
      char f = buffer[i];
      printf("f= %d ", (unsigned int) f);
      unsigned short l = (buffer[i + 1] & 255) + (buffer[i + 2] << 8);
      unsigned short a = (buffer[i + 3] & 255) + (buffer[i + 4] << 8);
      string s = decode(f, l, a);
      printf("%s\n",s.c_str() );
   }
}

/**
 * Print the tables
 *
 */

void Disassembler::print_tables()
{
   print_magic_number();
   uint16_t pc = find_offset();
   uint16_t start_ext_proc_table = find_ext_proc_table();
   printf("Method table starts at %04X\n",10);
   printf("Ext proc table starts at %04X\n",start_ext_proc_table);
   printf("Code starts at %04X\n",pc);
   //
   // fill the methodmap
   //
   for (uint16_t j = 10; j < start_ext_proc_table ; j += 8) {
      uint16_t classnum = (buffer[j] & 0xff) + ((buffer[j + 1] & 0xff) << 8);
      uint16_t methodnum = (buffer[j + 2] & 0xff) + ((buffer[j + 3] & 0xff) << 8);
      uint16_t address = (buffer[j + 4] & 0xff) + ((buffer[j + 5] & 0xff) << 8);
      uint16_t num_params = buffer[j + 6];
      uint16_t num_local_vars = buffer[j + 7];
      printf("Method classnum=%u methodnum=%u address=%04X num_params=%u num_local_vars=%u\n",classnum,methodnum,address,num_params,num_local_vars);
   }
   //
   // get the external symbols
   //
   uint16_t j = start_ext_proc_table;
   while (j < pc)
   {
      void* ptr;
      memcpy(&ptr,buffer + j,8); 
      j += 8;
      string signature = string(buffer + j);
      printf("External symbol address=%p\n",(void*)ptr);
      cout << "Signature " << signature << endl;
      j+= signature.length() + 1;
   } 
}

void Disassembler::print_magic_number() {
   printf("Magic Number %c%c%c%c%c%c\n", buffer[0] ,buffer[1], buffer[2], buffer[3] ,buffer[4] , buffer[5] );
}

uint16_t Disassembler::find_offset() {
   return buffer[6] + (buffer[7] >> 8);
}

uint16_t Disassembler::find_ext_proc_table() {
   return buffer[8] + (buffer[9] >> 8);
}


/**
 *
 * Hexdump of a buffer
 *
 */
void Disassembler::hexdump(char* buf, unsigned int buflen) {
   unsigned int i, j;
   for (i = 0; i < buflen; i += 16) {
      printf("%06x: ", i);
     for (j = 0; j < 16; j++)
     {
       unsigned int location = i + j;
         if (location < buflen)
       {
            printf("%02x ", buf[location] &255);
       }
       else
       {
          printf("   ");
       }
      printf(" ");
     }
     for (j = 0; j < 16; j++)
     {
        unsigned int location = i + j;
        if (location < buflen)
        {
           int a_char = buf[location] & 255;
           int prin = (a_char > 0) && isprint(a_char);
           printf("%c", prin ? a_char : '.');
        }
     }
     printf("\n");
   }
}

string Disassembler::decode(char f, unsigned short l, unsigned short a) {
//
// opcode definitions
//
   string sf, sl, sa;
   stringstream out1;
   out1 << l;
   sl = out1.str();
   stringstream out2;
   out2 << a;
   sa = out2.str();
   switch (f) {
   case 1:   // lit: Literal value, to be pushed on the stack
      sf = "LIT";
      switch (l) {
      case 2: // Int
         i += 5;
         break;
      case 5: // float
         i += 13;
         break;
      case 7: // string
         //
         i += 5 + a;
         break;
      case 6: // boolean
         i += 5;
         break;
      case 0: // null
         i +=5;
         break;
      default:
         printf("unexpected LIT value");
         i += 5;
         break;
      }
      break;
   case 2: // opr
      sf = "OPR";
      switch (a) {
      case 0:
         sa = "RET";
         break;
      case 1:
         sa = "UNARY MINUS";
         break;
      case 2:
         sa = "PLUS";
         break;
      case 3:
         sa = "MINUS";
         break;
      case 4:
         sa = "MUL";
         break;
      case 5:
         sa = "DIV";
         break;
      case 6:
         sa = "MOD";
         break;
      case 8:
         sa = "EQ";
         break;
      case 9:
         sa = "NE";
         break;
      case 10:
         sa = "LT";
         break;
      case 11:
         sa = "GE";
         break;
      case 12:
         sa = "GT";
         break;
      case 13:
         sa = "LE";
         break;
      default:
         sa = "unexpected A value: " + a;
         printf("the a value was unexpected <%d>\n",a);
         break;
      }
      i += 5;
      break;
   case 3:
      sf = "LOD";
      //lod: copy a local variable on top of the stack
      i += 5;
      break;
   case 4:
      // sto: pop a value from the stack and put it in a local variable
      sf = "STO";
      i += 5;
      break;
   case 5:
      //cal:
      // parameters should have already been pushed on the stack
      // push the return address on the return stack
      // call the procedure
      //
      sf = "CAL";
      i += 5;
      break;
   case 6:
      // int:
      sf = "INT";
      i += 5;
      break;
   case 7:
      // jmp
      sf = "JMP";
      i += 5;
      break;
   case 8:
      // jpc
      sf = "JPC";
      i += 5;
      break;
   case 9:
      sf = "PRINT";
      i += 5;
      break;
   case 10:
      sf = "DYNCAL";
      i += 5;
      break;
   case 11:
      sf = "OBJ CREATION";
      i += 5;
      break;
   case 12:
      sf = "METHOD CALL";
      i += 5;
      break;
   case 13:
      sf = "INSTVAR";
      i +=5;
      break;
   case 15:
      sf = "DROP";
      i += 5;
      break;
   default:
      sf = "-----------------------------------+> unexpected F value: " + int(f);
      printf(" F=%d L=%d A=%d" , (unsigned int) f , l , a );
      i += 5;
      break;
   }
   string total = sf + " " + sl + "," + sa;
   return total;
}
