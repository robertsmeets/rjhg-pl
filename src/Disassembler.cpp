/*
 * Disassembler.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

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
   i = (buffer[6] & 0xff) + ((buffer[7] & 0xff) >> 8);
   printf("---------------------------------------------------------\n");
   printf("i is now %d size is now %d\n",i,size);
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
      printf("External symbol address=%p\n",ptr);
      printf("fopen=%p\n",fopen);
      printf("Signature %s\n",signature.c_str());
      j+= signature.length() + 1;
   } 
}

void Disassembler::print_magic_number() {
   printf("Magic Number %c%c%c%c%c%c\n", buffer[0] ,buffer[1], buffer[2], buffer[3] ,buffer[4] , buffer[5] );
}

uint16_t Disassembler::find_offset() {
   uint16_t* uptr = (uint16_t*) (buffer + 6);
   return *uptr;
}

uint16_t Disassembler::find_ext_proc_table() {
   uint16_t* uptr = (uint16_t*) (buffer + 8);
   return *uptr;
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
            printf("%02x ", buf[location] & 0xff);
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
           int a_char = buf[location] & 0xff;
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
   case OPCODE_LIT:   // lit: Literal value, to be pushed on the stack
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
   case OPCODE_I:
         sf = "I";
      i += 5;
         break;
      case OPCODE_NOT:
         sf = "NOT";
      i += 5;
         break;
      case OPCODE_AND:
         sf = "AND";
      i += 5;
         break;
      case OPCODE_OR:
         sf = "OR";
      i += 5;
         break;
   case OPCODE_LOD:
      sf = "LOD";
      //lod: copy a local variable on top of the stack
      i += 5;
      break;
   case OPCODE_STO:
      // sto: pop a value from the stack and put it in a local variable
      sf = "STO";
      i += 5;
      break;
   case OPCODE_CAL:
      //cal:
      // parameters should have already been pushed on the stack
      // push the return address on the return stack
      // call the procedure
      //
      sf = "CAL"; // procedure call
      i += 5;
      break;
   case OPCODE_INT:
      // int:
      sf = "INT";
      i += 5;
      break;
   case OPCODE_JMP:
      // jmp
      sf = "JMP";
      i += 5;
      break;
   case OPCODE_JPC:
      // jpc
      sf = "JPC";
      i += 5;
      break;
   case OPCODE_PRT:
      sf = "PRT";
      i++;
      break;
   case OPCODE_EXT:
      sf = "EXT";
      i += 5;
      break;
   case OPCODE_OBJ:
      sf = "OBJ";
      i += 5;
      break;
   case OPCODE_MCL:
      sf = "MCL"; // method call
      i += 5;
      break;
   case OPCODE_LDI:
      sf = "LDI";
      i +=5;
      break;
   case OPCODE_STI:
      sf = "STI";
      i +=5;
      break;
   case OPCODE_DRP:
      sf = "DRP";
      i ++;
      break;
   case OPCODE_JPF:
      sf = "JPF";
      i += 5;
      break;
   case OPCODE_SLF: 
      sf = "SLF";
      i += 5;
      break;
   case OPCODE_RET: 
      sf = "RET";
      i += 3;
      break;
   case OPCODE_PLS:
      sf = "PLS";
      i += 5;
      break;
   case OPCODE_MIN:
      sf = "MIN";
      i += 5;
      break;
   case OPCODE_MUL:
      sf = "MUL";
      i += 5;
      break;
   case OPCODE_UNA: 
        sf = "UNA"; // UNARY MINUSs
        i+=5;
        break;
   case OPCODE_DIV:
        sf = "DIV";
        i += 5;
        break;
   case OPCODE_MOD:
        sf = "MOD";
        i += 5;
        break;
   case OPCODE_EQ: sf = "EQ"; i+=5;break; 
   case OPCODE_NE: sf = "NE"; i+=5;break;
   case OPCODE_LT: sf = "LT"; i+=5;break;
   case OPCODE_GE: sf = "GE"; i+=5;break;
   case OPCODE_GT: sf = "GT"; i+=5;break;
   case OPCODE_LE: sf = "LE"; i+=5;break;

   default:
      sf = "-----------------------------------+> unexpected F value: " + int(f);
      printf(" F=%d L=%d A=%d" , (unsigned int) f , l , a );
      i += 5;
      break;
   }
   string total = sf + " " + sl + "," + sa;
   return total;
}
