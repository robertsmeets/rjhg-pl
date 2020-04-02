/*
 * Disassembler.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: robert
 */

#include "Disassembler.h"

using namespace std;

Disassembler::Disassembler() {
   i = 0;
   buffer = 0;
}

void Disassembler::start(char* inbuffer, unsigned int size) {
   printf("NOT Starting disassembler...\n" );
   return ;
   if (strncmp(inbuffer,"RJHGPL",6) != 0) { printf("Magic number does not match, invalid bytecode"); exit(-1); }
   //
   // hexdump
   //
   buffer = inbuffer;
   hexdump(size);
   print_tables();
   for (i = *((uint16_t*)(inbuffer + 6));i < size;) {
      printf("%04X ",i );
      decode();
      printf("%04X ",i );
   }
   printf("Finishing disassembler\n"); 
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
void Disassembler::hexdump(unsigned int buflen) {
   unsigned int i, j;
   for (i = 0; i < buflen; i += 16) {
     printf("%06x: ", i);
     for (j = 0; j < 16; j++)
     {
        unsigned int location = i + j;
        if (location < buflen)
        {
            printf("%02x ", buffer[location] & 0xff);
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
           int a_char = buffer[location] & 0xff;
           int prin = (a_char > 0) && isprint(a_char);
           printf("%c", prin ? a_char : '.');
        }
     }
     printf("\n");
   }
}

string Disassembler::decode() {
   //
   // opcode definitions
   //
   char* address = buffer + i;
   char f = *address;
   uint16_t l = *(uint16_t*)(address + 1);
   uint16_t a = *(uint16_t*)(address + 3);
   switch (f) {
      case OPCODE_LINT: { printf("LINT\n"); i += 5; break; }
      case OPCODE_LIT:   // lit: Literal value, to be pushed on the stack
         {
            printf("LIT");
            switch (l) {
                case 0: printf(" NULL\n"); i +=5; break;
                case 5: printf(" FLOAT\n"); i += 13; break;
                case 7: printf(" STRING\n"); i += 5 + a; break;
                case 6: printf( " BOOL\n"); i += 5; break;
                default: printf("unexpected LIT value\n"); i += 5; break;
            }
            break; 
         }
      case OPCODE_I: printf("I\n"); i ++; break;
      case OPCODE_NOT: printf( "NOT\n"); i ++; break;
      case OPCODE_AND: printf( "AND\n"); i ++; break;
      case OPCODE_OR: printf( "OR\n"); i ++; break;
      case OPCODE_LOD: printf( "LOD %d\n",a); i += 5; break; //lod: copy a local variable on top of the stack
      case OPCODE_STO: printf( "STO %d\n",a); i += 5; break; // sto: pop a value from the stack and put it in a local variable
      case OPCODE_CAL: printf( "CAL %04X\n",a); i += 5; break; // procedure call 
      case OPCODE_INT: printf( "INT %d %d\n",l,a); i += 5; break; // int:
      case OPCODE_JMP: printf("JMP %04X\n",l); i += 3; break; // jmp
      case OPCODE_JPC: printf("JPC %04X\n",l); i += 3; break; // jpc
      case OPCODE_JPF: printf("JPF %04X\n",l); i += 3; break;
      case OPCODE_PRT: printf( "PRT\n"); i++; break;
      case OPCODE_EXT: printf( "EXT %d %d\n",l,a); i += 5; break;
      case OPCODE_OBJ: printf( "OBJ %d %d\n" ,l,a); i += 5; break; 
      case OPCODE_MCL: printf( "MCL %d %d\n" ,l,a); i += 5; break; // method call
      case OPCODE_LDI: printf( "LDI %d %d\n" ,l,a); ; i +=5; break;
      case OPCODE_LDG: printf( "LDG %d %d\n" ,l,a); ; i +=5; break;
      case OPCODE_STI: printf( "STI %d %d\n" ,l,a); ; i +=5; break;
      case OPCODE_STG: printf( "STG %d %d\n" ,l,a); ; i +=5; break;
      case OPCODE_DRP: printf( "DRP\n"); i ++; break;
      case OPCODE_SLF: printf( "SLF %d %d\n",l,a); i += 5; break;
      case OPCODE_RET: printf( "RET %d\n" ,l);  i += 3; break;
      case OPCODE_PLS: printf( "PLS\n"); i ++; break;
      case OPCODE_MIN: printf( "MIN\n"); i ++; break;
      case OPCODE_MUL: printf( "MUL\n"); i ++; break;
      case OPCODE_UNA: printf( "UNA\n"); i++; break; // UNARY MINUS 
      case OPCODE_DIV: printf( "DIV\n") ; i ++; break;
      case OPCODE_MOD: printf( "MOD\n"); i ++ ; break;
      case OPCODE_EQ: printf( "EQ\n"); i++;break; 
      case OPCODE_NE: printf( "NE\n"); i++;break;
      case OPCODE_LT: printf( "LT\n"); i++;break;
      case OPCODE_GE: printf( "GE\n"); i++;break;
      case OPCODE_GT: printf( "GT\n"); i++;break;
      case OPCODE_LE: printf( "LE\n"); i++;break;
   default:
     {
      printf( "-----------------------------------+> unexpected F value: %d",f);
      printf(" F=%d L=%d A=%d" , f , l , a );
      exit(-1);
      }
   }
}
