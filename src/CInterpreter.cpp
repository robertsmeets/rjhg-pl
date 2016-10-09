/*
 * CInterpreter.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#include "CInterpreter.h"
#include "stdint.h"

using namespace std;

CInterpreter::CInterpreter(char* a_buffer, DebugInfo* a_di) {
   hm = new HeapManager();
   hm->setInterpreter(this);
   di = a_di;
   buffer = a_buffer;
   pc = 0; // program counter
   t = 0;   // is the top of the stack s
   tr = 0;  // is the top of the stack r
   tb = 0;  // is the top of the stack b
   s = vector<stack_element>(500);
   r = vector<unsigned short int>(500); // return stack
   b = vector<unsigned short int>(500); // block address stack
   //
   // set up a call table matrix
   // entries; first the operator 0..13
   // second the first argument type 1..7
   // third the second argument type 1..7
   //
   // The result is a function
   //

   //
   // fill up for OPR 2 (PLUS);
   //
   fptrs[2][2][2] = (iiptr) (&func_plus_ii);
   fptrs[2][2][5] = (iiptr) (&func_plus_id);
   fptrs[2][5][2] = (iiptr) (&func_plus_di);
   fptrs[2][5][5] = (iiptr) (&func_plus_dd);
   //
   // fill up for OPR 3 (MINUS);
   //
   fptrs[3][2][2] = (iiptr) (&func_minus_ii);
   fptrs[3][2][5] = (iiptr) (&func_minus_id);
   fptrs[3][5][2] = (iiptr) (&func_minus_di);
   fptrs[3][5][5] = (iiptr) (&func_minus_dd);
   //
   // fill up for OPR 4 (MUL);
   //
   fptrs[4][2][2] = (iiptr) (&func_mul_ii);
   fptrs[4][2][5] = (iiptr) (&func_mul_id);
   fptrs[4][5][2] = (iiptr) (&func_mul_di);
   fptrs[4][5][5] = (iiptr) (&func_mul_dd);
   //
   // fill up for OPR 8..13
   //
   fptrs[8][2][2] = (iiptr) (&func_eq_ii);
   fptrs[8][2][5] = (iiptr) (&func_eq_id);
   fptrs[8][5][2] = (iiptr) (&func_eq_di);
   fptrs[8][5][5] = (iiptr) (&func_eq_dd);

   fptrs[9][2][2] = (iiptr) (&func_ne_ii);
   fptrs[9][2][5] = (iiptr) (&func_ne_id);
   fptrs[9][5][2] = (iiptr) (&func_ne_di);
   fptrs[9][5][5] = (iiptr) (&func_ne_dd);

   fptrs[10][2][2] = (iiptr) (&func_lt_ii);
   fptrs[10][2][5] = (iiptr) (&func_lt_id);
   fptrs[10][5][2] = (iiptr) (&func_lt_di);
   fptrs[10][5][5] = (iiptr) (&func_lt_dd);

   fptrs[11][2][2] = (iiptr) (&func_ge_ii);
   fptrs[11][2][5] = (iiptr) (&func_ge_id);
   fptrs[11][5][2] = (iiptr) (&func_ge_di);
   fptrs[11][5][5] = (iiptr) (&func_ge_dd);

   fptrs[12][2][2] = (iiptr) (&func_gt_ii);
   fptrs[12][2][5] = (iiptr) (&func_gt_id);
   fptrs[12][5][2] = (iiptr) (&func_gt_di);
   fptrs[12][5][5] = (iiptr) (&func_gt_dd);

   fptrs[13][2][2] = (iiptr) (&func_le_ii);
   fptrs[13][2][5] = (iiptr) (&func_le_id);
   fptrs[13][5][2] = (iiptr) (&func_le_di);
   fptrs[13][5][5] = (iiptr) (&func_le_dd);

}

CInterpreter::~CInterpreter() {
   delete hm;
}

int CInterpreter::getStackDepth()
{
   return t;
}

vector<stack_element>* CInterpreter::getStack() {
   return &s;
}

void CInterpreter::start(bool indebug) {
   debug=indebug;
   if (debug) {printf("Starting interpreter...\n" );}
   methodmap.clear();
   check_magic_number();
   pc = find_offset();
   if (debug){ printf("PC is now 0x%x\n",pc);}
   uint16_t start_ext_proc_table = find_ext_proc_table();
   //
   // fill the methodmap
   //
   for (uint16_t j = 10; j < start_ext_proc_table ; j += 8) {
      uint16_t classnum = (buffer[j] & 0xff) + ((buffer[j + 1] & 0xff) << 8);
      uint16_t methodnum = (buffer[j + 2] & 0xff) + ((buffer[j + 3] & 0xff) << 8);
      uint16_t address = (buffer[j + 4] & 0xff) + ((buffer[j + 5] & 0xff) << 8);
      uint16_t num_params = buffer[j + 6];
      uint16_t num_local_vars = buffer[j + 7];
      auto k = methodmap.find(methodnum);
      if (k == methodmap.end()) {
         //
         // was not found
         //
         methodmap[methodnum] = map<uint16_t, uint16_t[3]>();
      }
      methodmap[methodnum][classnum][0] = address;
      methodmap[methodnum][classnum][1] = num_params;
      methodmap[methodnum][classnum][2] = num_local_vars;
   }
   //
   // get the external symbols
   //
   externs.clear();
   uint16_t j = start_ext_proc_table;
   while (j < pc)
   {
       void* ptr;
       memcpy(&ptr,buffer + j,8); 
      j += 8;
      extern_record er;
      string signature = string(buffer + j);
      er.address = (long long unsigned int)ptr;
      er.signature = signature; 
      externs.push_back(er);
      j+= signature.length() + 1;
   } 
   unsigned i = 0;
   for (; !i;) {
      i = execute_next(debug);
   }
}

void CInterpreter::check_magic_number() {
   if (!(buffer[0] == 'R' && buffer[1] == 'J' && buffer[2] == 'H'
         && buffer[3] == 'G' && buffer[4] == 'P' && buffer[5] == 'L')) {
      puts("Magic number does not match, invalid bytecode");
   }
}

uint16_t CInterpreter::find_offset() {
   return buffer[6] + (buffer[7] >> 8);
}

uint16_t CInterpreter::find_ext_proc_table() {
   return buffer[8] + (buffer[9] >> 8);
}

/*

 p, b, t: integer; {program-, base-, topstack-registers}
 i: instruction; {instruction register}
 s: array [1..stacksize] of integer; {datastore}
 */
int CInterpreter::execute_next(bool debug) {
   //
   // f is the opcode
   // l is the 2nd param
   // a is the 3rd param
   //
if (debug) {
   printf("pc=0x%x: ", pc );
}
   unsigned short int f = *((char*) buffer + pc) & 0xff;
   pc++;
   //
   // using little endian
   //
   char* lptr = (char*) buffer + pc;
   unsigned short int l = (*lptr & 0xff) + (*(lptr + 1) << 8);
   pc += 2;
   lptr = (char*) buffer + pc;
   unsigned short int a = (*lptr & 0xff) + (*(lptr + 1) << 8);
   pc += 2;
   stack_element temp;
   //
   // opcode definitions
   //
   char* ptr;
   double d1;
   double d2;
   double d3;
   stack_element fr1;
   stack_element fr2;
   uint16_t adr;
   uint16_t classnum;
   switch (f) {
   case 1:   // lit: Literal value, to be pushed on the stack
if (debug) {
   printf("LIT %d,%d", l , a);
}
      switch (l) {
      case 0: // null
         s[t].atype=0;
         s[t].address = 0;
         t++;
         break;
      case 2: // Int
         s[t].atype = 2;
         s[t].address = a;
         t++;
         break;
      case 5: // float
         ptr = hm->allocate(a);
         memcpy(ptr, buffer + pc, a);
         //
         // put the pointer and the type on the stack
         //
         s[t].atype = 5;
         s[t].address = (uint16_t) (ptr - hm->getStart());
         t++;
         pc += a;
         break;
      case 6: // boolean
         s[t].atype = 6;
         s[t].address = a;
         t++;
         break;
      case 7: // string
      {
         //
         // get some memory
         //
         char* tmp = hm->allocate(a + 2);
         print_stack();
         //
         // copy the string to the heap
         //
         *tmp = a & 0xff;
	 *(tmp + 1) = (a >> 8) & 0xff;
	 print_stack();
         memcpy(tmp + 2, buffer + pc, a);
         //
         // put the pointer and the type on the stack
         //
         print_stack();
         s[t].atype = 7;
         s[t].address = (uint16_t) (tmp - hm->getStart());
         t++;
         pc += a;
         print_stack();
         break;
      }
      default:
         puts("unexpected LIT value " + s[t].atype);
      }
      break;
   case 2: // opr
      if (debug) { printf("OPR"); }

      iiptr aiiptr;
      idptr aidptr;
      diptr adiptr;
      ddptr addptr;

      biiptr abiiptr;
      bidptr abidptr;
      bdiptr abdiptr;
      bddptr abddptr;

      switch (a) {
      case 0:
         if (debug) { printf(" RET l= %d" , l); }
         if (tr <= 0) {
            if(debug){printf("Exiting program...\n" );}
            // exit
            return -1;
         }
         tr--;
         pc = r[tr];
         //
         // also free the block
         //
         // there may be a return value on the top of the stack that needs to be saved
         //
         if (l > 0) {
            temp = s[t - 1];
         }
         t = b[tb - 1];
         tb--;
         if (l > 0) {
            s[t] = temp;
            t++;
         }
         break;
      case 1:
         if (debug) { printf(" UNARY MINUS"); }
         fr1 = s[t - 1];
         if (fr1.atype != 2) {
            puts("type must be integer");
         }
         fr1.address = -fr1.address;
         s[t - 1] = fr1;
         break;
      case 2:
      case 3:
      case 4:
         if (debug) { printf(" PLUS, MINUS or MUL"); }
         t--;
         fr1 = s[t - 1];
         fr2 = s[t];
         if ((fr1.atype == 2) && (fr2.atype == 2)) {
            //
            // operation on two integers
            //
            aiiptr = fptrs[a][fr1.atype][fr2.atype];
            fr1.address = (*aiiptr)(fr1.address, fr2.address);
            s[t - 1] = fr1;

         } else if ((fr1.atype == 7) && (fr2.atype == 7)) {
            //
            // add two strings
            //
            char * ptr1 = hm->getStart() + fr1.address;
            char * ptr2 = hm->getStart() + fr2.address;
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            uint16_t len2 = ((*ptr2) & 0xff) + (*(ptr2 + 1) << 8);
            uint16_t newlen = len1 + len2;
            ptr = hm->allocate(newlen + 2);
            *ptr = newlen & 0xff;
            *(ptr + 1) = newlen >> 8;
            memcpy(ptr + 2, ptr1 + 2, len1);
            memcpy(ptr + len1 + 2, ptr2 + 2, len2);
            fr1.address = (uint16_t) (ptr - hm->getStart());
            s[t - 1] = fr1;
         } else if ((fr1.atype == 7) && (fr2.atype == 2)) {
            //
            // add a string and an integer
            //
            char* ptr1 = hm->getStart() + fr1.address;
            char str[15];
            snprintf(str, 15, "%d", fr2.address);
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            uint16_t len2 = strlen(str);
            uint16_t newlen = len1 + len2;
            ptr = hm->allocate(newlen + 2);
            *ptr = newlen & 0xff;
            *(ptr + 1) = newlen >> 8;
            memcpy(ptr + 2, ptr1 + 2, len1);
            memcpy(ptr + len1 + 2, &str, len2);
            fr1.address = (uint16_t) (ptr - hm->getStart());
            s[t - 1] = fr1;
         } else if ((fr1.atype == 2) && (fr2.atype == 5)) {
            //
            // integer plus float
            //
            char* st = hm->getStart();
            memcpy(&d2, st + fr2.address, 8);
            aidptr = (idptr) (fptrs[a][fr1.atype][fr2.atype]);
            d3 = (*aidptr)(fr1.address, d2);
            stack_element* fr3 = new stack_element();
            fr3->atype = 5;
            char* tmp = hm->allocate(8);
            fr3->address = tmp - hm->getStart();
            memcpy(st + fr3->address, &d3, 8);
            s[t - 1] = *fr3;
         } else if ((fr1.atype == 5) && (fr2.atype == 2)) {
            //
            // float plus integer
            //
            char* st = hm->getStart();
            memcpy(&d1, st + fr1.address, 8);
            adiptr = (diptr) (fptrs[a][fr1.atype][fr2.atype]);
            d3 = (*adiptr)(d1, fr2.address);
            stack_element* fr3 = new stack_element();
            fr3->atype = 5;
            char* tmp = hm->allocate(8);
            fr3->address = tmp - hm->getStart();
            memcpy(st + fr3->address, &d3, 8);
            s[t - 1] = *fr3;

         } else if ((fr1.atype == 5) && (fr1.atype == 5)) {
            //
            // both floats
            //
            char* st = hm->getStart();
            //
            // copy both floats to temp variables d1 and d2
            //
            memcpy(&d1, st + fr1.address, 8);
            memcpy(&d2, st + fr2.address, 8);
            //
            // find the function
            //
            addptr = (ddptr) (fptrs[a][fr1.atype][fr2.atype]);
            //
            // perform the operation
            //
            d3 = (*addptr)(d1, d2);
            //
            //
            stack_element* fr3 = new stack_element();
            fr3->atype = 5;
            char* tmp = hm->allocate(8);
            fr3->address = tmp -hm->getStart();
            memcpy(st + fr3->address, &d3, 8);
            s[t - 1] = *fr3;
         } else {
            printf("operation %d incompatible types %d and %d\n",a,fr1.atype,fr2.atype);
            exit(-1);
         }
         break;
      case 5:
if (debug) {
         printf(" DIV");
}
         t--;
         fr1 = s[t - 1];
         fr2 = s[t];
         if ((fr1.atype != 2) || (fr2.atype != 2)) {
            puts("division both types must be integer");
         }
         fr1.atype = 2;
         fr1.address = fr1.address / fr2.address;
         s[t - 1] = fr1;
         break;
      case 6:
if (debug) {
         printf(" MOD");
}
         t--;
         fr1 = s[t - 1];
         fr2 = s[t];
         if ((fr1.atype != 2) || (fr2.atype != 2)) {
            puts("modulo both types must be integer");
         }
         fr1.atype = 2;
         fr1.address = fr1.address % fr2.address;
         s[t - 1] = fr1;
         break;
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
         t--;
         fr1 = s[t - 1];
         fr2 = s[t];
         if ((fr1.atype == 2) && (fr2.atype == 2)) {
            //
            // operation on two integers
            //
            abiiptr = (biiptr) fptrs[a][fr1.atype][fr2.atype];
            bool eq = (*abiiptr)(fr1.address, fr2.address);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } else if ((fr1.atype == 2) && (fr2.atype == 5)) {
            //
            // integer plus float
            //
            char* st = hm->getStart();
            memcpy(&d2, st + fr2.address, 8);
            abidptr = (bidptr) (fptrs[a][fr1.atype][fr2.atype]);
            bool eq = (*abidptr)(fr1.address, d2);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } else if ((fr1.atype == 5) && (fr2.atype == 2)) {
            //
            // float plus integer
            //
            char* st = hm->getStart();
            memcpy(&d1, st + fr1.address, 8);
            abdiptr = (bdiptr) (fptrs[a][fr1.atype][fr2.atype]);
            bool eq = (*abdiptr)(d1, fr2.address);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } else if ((fr1.atype == 5) && (fr1.atype == 5)) {
            //
            // both floats
            //
            char* st = hm->getStart();
            memcpy(&d1, st + fr1.address, 8);
            memcpy(&d2, st + fr2.address, 8);
            abddptr = (bddptr) (fptrs[a][fr1.atype][fr2.atype]);
            bool eq = (*abddptr)(d1, d2);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } 
          else if ((fr1.atype == 0) && (fr2.atype == 8)) {
            //
            // null vs pointer 
            //
            bool eq = (fr2.address == 0);
            fr1.atype=6;
            fr1.address=eq;
            s[t-1]= fr1;
          }
          else if ((fr1.atype == 8) && (fr2.atype == 0)) {
            //
            // pointer vs null
            //
            bool eq = (fr1.address == 0);
            fr1.atype=6;
            fr1.address=eq;
            s[t-1]= fr1;
          }
         else {
            printf( "operation %d: incompatible types %d and %d\n",a,fr1.atype,fr2.atype);
            exit(-1);
         }
         break;
      case 14:
         {
            printf("Index operation. Don't know what to do yet\n");
            exit(-1);
         }
      default:
         puts("unexpected A value");
         return -1;
         break;
      }
      break;
   case 3:
      if (debug) { printf("LOD %d ",a); }
      //
      // lod: copy a local variable or parameter on top of the stack
      //
      s[t] = s[b[tb - 1] + a];
      t++;
      break;
   case 4:   // sto: pop a value from the stack and put it in a local variable or parameter
      if (debug) { printf("STO %d " ,a); }
      t--;
      s[b[tb - 1] + a] = s[t];
      break;
   case 5:         //cal:
      // parameters should have already been pushed on the stack
      // push the return address on the return stack
      // call the procedure
      //
      if (debug) { printf("CAL %d ",a); }
      r[tr] = pc;
      tr++;
      pc = a;
      break;
   case 6:         // int:
      if (debug) { printf("INT %d,%d", l , a); }
      //
      // this creates a new block with depth a for local variables and parameters
      //
      b[tb] = t - l;
      tb++;
      //
      // l contains the number of parameters
      // a contains the number of local variables
      //
      // only add a because the parameters will be pushed on the stack
      //
      t += a;
      break;
   case 7:         // jmp
      if (debug) { printf("JMP %d " , a); }
      pc = a;
      break;
   case 8:         // jpc - jump when true
      if (debug) { printf("JPC %d " , a); }
      fr1 = s[t - 1];
      if (fr1.atype != 6) {
         puts("JPC value is not boolean");
      }
      if (fr1.address != 0) {
         pc = a;
      }
      t--;
      break;
   case 9: // print
      if (debug) { printf("PRINT %d " , a); }
      t--;
      fr1 = s[t];
      if (fr1.atype == 7) {
         char* ptr = hm->getStart() + fr1.address;
         print_a_string(ptr,true);
      } else if (fr1.atype == 5) {
         //
         // float
         //
         char* ptr = hm->getStart() + fr1.address;
         memcpy(&d1, ptr, 8);
         printf("%f\n",d1 );
      } else if (fr1.atype == 2) {
         printf("%d\n",fr1.address );
      } else if (fr1.atype == 6) { // boolean
         if (fr1.address) {
            printf("true\n" );
         } else {
            printf("false\n" );
         }
      } else if (fr1.atype == 8) {
         printf("A Pointer\n" );
      } else if (fr1.atype == 9) {
         printf("A Fancy Object\n" );
      } else {
         printf( "Cannot print something of type %d\n", fr1.atype );
      }
      break;

   case 10: // external function call
if (debug) {
   printf("EXTCALL %d",  a);
}
      // parameters should have already been pushed on the stack
      //
      //
      // call the procedure
      //
      call_external(l,a);
      break;
   case 11: // object creation
      if (debug) { printf("OBJCREATE %d,%d " ,l,a); }
      //
      // l contains the classnum
      // a contains the number of instance variables
      //
      ptr = hm->allocate(3 * a + 3);
      //
      // in the first 2 bytes, put in the class number
      // the rest is left for the instance variables
      //
      *ptr = l & 0xff;
      *(ptr + 1) = l >> 8;
      *(ptr + 2) = a;
      //
      // leave the new object on the stack
      //
      s[t].atype = 8; // Object?
      s[t].address = ptr - hm->getStart();
      t++;
      break;
   case 12:
      if (debug) { printf("METHODCALL %d " ,l ); }
      // method call, the object is already on the stack.
      //
      // lookup the method by object type and name
      // call the method
      //
      // l is the method number
      // a is the number of parameters
      //
      // figure out what class type is on top of the stack
      //
      if (s[t-1].atype != 8) {
         puts("Performed a method call on a nonfancy object");
         return -1;
      }
      ptr = hm->getStart() + s[t-1].address;
      classnum = (*ptr & 0xff) + (*(ptr + 1) << 8);
      if (debug) { printf("classnum = %d ", classnum ); }
      //
      // this creates a new block with depth for local variables and parameters
      //
      b[tb] = t - methodmap[l][classnum][1] - a;
      tb++;
      //
      // add room for the this pointer and the local vars
      //
      t += methodmap[l][classnum][2] + 1;
      //
      // add the program counter on the return stack
      //
      r[tr] = pc;
      tr++;
      pc = methodmap[l][classnum][0];
      break;
   case 13:
      if (debug) { printf("LDI %d ", l ); }
      // access an instance variable and put it on the stack
      //
      // l is the instance variable
      // a is the number of parameters
      //
      // first get the this pointer
      // then calculate the address of the inst. variable
      // But what is the offset of the this pointer?
      //
      adr = s[b[tb - 1] + a].address + 3 * l + 3;
      if (debug){printf("-----The offset is %d\n",adr);}
      //
      // put the value on the stack
      //
      s[t].atype = *(hm->getStart() + adr) & 0xff;
      if (debug){printf("-----The type is %d\n",s[t].atype);}
      s[t].address = (*(hm->getStart() + adr + 1) & 0xff) + (*(hm->getStart() + adr + 2) << 8);
      if (debug){printf("-----The object is %d\n",s[t].address);}
      t++;
      break;
   case 14:
      if (debug) { printf("STI %d %d", l,a ); }
      // store a value inside an inst. variable
      //
      // l is the instance variable
      // a is the number of parameters
      //
      // first get the this pointer
      // then calculate the address of the inst. variable
      //
      adr = s[b[tb - 1] + a].address + 3 * l + 3;
      if(debug){printf("offset = %d\n",adr);}
      //
      // store the value on the heap
      //
      *(hm->getStart() + adr) = s[t-1].atype;
      *(hm->getStart() + adr + 1) = s[t-1].address & 0xff;
      *(hm->getStart() + adr + 2) = s[t-1].address >> 8;
      if (debug){printf("-----The type is %d\n",s[t-1].atype);}
      if (debug){printf("-----The object is %d\n",s[t-1].address);}
      t--;
      break;
   case 15:
      if (debug) { printf("DROP"); }
      //
      // drop the top value of the stack
      //
      t--;
      break;
   default:
      printf("unexpected F value <0x%x> at PC <0x%x>\n",f,pc);
      exit(-1);
      break;
   }
   print_stack();
   return 0;
}


void CInterpreter::print_stack()
{
if (debug) {
   //
   // print the stack
   //
   printf("      stack: ");
   for (uint16_t i = 0; i < t; i++) {
      char* adr;
      printf("[");
      switch (s[i].atype) {
         case 0:
            printf("NULL");
            break;
         case 2:
            printf("%d",s[i].address);
            break;
         case 5:
            adr = hm->getStart() + s[i].address;
            double d;
            memcpy(&d, adr, 8);
            printf("%f",d);
            break;
         case 6:
            if (s[i].address)
            {
               printf("true");
            }
            else
            {
               printf("false");
            }
            break;
         case 7: // string
            adr = hm->getStart() + s[i].address;
            print_a_string(adr,false);
            break;
         case 8: // pointer
         {
            printf("ptr %p",s[i].address);
            break;
         }
         case 9: // object reference
            printf("objref");
            break;
         default:
            printf("?%d" , s[i].atype);
            break;
      }
      printf("]");
   }
   printf("      bstack: ");
   for (uint16_t i = 0; i < tb; i++) {
      printf("%d ",b[i]);
   }
   printf("      rstack: ");
   for (uint16_t i = 0; i < tr; i++) {
      printf("0x%x", r[i]);
   }
   printf("\n");
}
}

void CInterpreter::print_a_string(char* ptr,bool b) {
   uint16_t len = ((*ptr) & 0xff) + (*(ptr + 1) << 8);
   print_a_string(ptr + 2, len);
   if (b)
   {
      printf("\n");
   }

}

void CInterpreter::print_a_string(char* ptr, uint16_t len) {
   for (char* i = ptr; i < ptr + len; i++) {
      printf("%c",*i);
   }
}

/**
 *
 * call an external function
 *
 **/
void CInterpreter::call_external(short unsigned int function_number,short unsigned int a) {
   if (function_number > externs.size())
   {
        printf("illegal function number %d highest function number is %d\n",function_number,externs.size());
        exit(-1);
   }
   extern_record e = externs[function_number-1];
   void* sym = (void*)(e.address);
   string signature = e.signature;
   size_t pos = signature.find("-");
   bool varargs =  (pos==std::string::npos);
   if (varargs)
   {
      pos = signature.find("+");
   }
   string ingoing =  signature.substr(0,pos);
   int ilen = ingoing.length();
   string outgoing = signature.substr(pos+1); 
   DCCallVM* vm = dcNewCallVM(4096);
   if (varargs)
   {
      if (a < ilen) 
      {
         printf("Mismatch: ingoing arguments given %d but expected at least %d",a,ingoing.size());
         exit(-1);
      }
      dcMode(vm, DC_CALL_C_ELLIPSIS);
   }
   else
   {
      if (a != ilen) 
      {
         printf("Mismatch: ingoing arguments given %d but expected %d",a,ingoing.size());
         exit(-1);
      }
      dcMode(vm, DC_CALL_C_DEFAULT);
   }
   dcReset(vm);
   //
   // loop over the given args
   //
   int nr_ingoing = ilen;
   int cnt = a;
   for(char& c : ingoing) {
      stack_element f = s[t - cnt];
      pass_in_arg(vm,c,f);
      cnt--;
   } 
   //
   //  in case of a vararg, there may be more args
   //
   int left = a - ilen;
   for (int i = 0; i< left;i++)
   {
      stack_element f = s[t - cnt];
      pass_in_arg(vm,' ' ,f);
      cnt--;
   }
   t -= a;
   char c = outgoing[0];
   //
   // do the actual call
   //
   switch (c)
   {
      case 'd':
      {
         double r = dcCallDouble(vm, sym);
         //
         // put the result on the stack
         //
         char* ptr = hm->allocate(8);
         memcpy(ptr, &r, 8);
         tb--;
         s[t].atype = 5;
         s[t].address = (uint16_t) (ptr - hm->getStart());
         t++;
         break;
      }
      case 'p':
      {
          DCpointer r = dcCallPointer(vm, sym);
          //
          // put the result on the stack
          //
          tb--;
          s[t].atype = 8;
          s[t].address = (long long unsigned int)r;
          t++;
         break;
      }
      case 'v':
      {
         //
         // no return value
         //
         dcCallVoid(vm,sym);
         break;
      }
      case 'i': 
      {  
         // 
         // integer return type
         //
         int i = dcCallInt(vm,sym);
         tb--;
         s[t].atype = 2;
         s[t].address = i;
         t++;
         break;
      }
      default:
         printf("unexpected return type <%c> in external call",c);
         exit(-1);

   }
   dcFree(vm);
}


void CInterpreter::pass_in_arg( DCCallVM* vm, char c,stack_element f)
{
      uint16_t atype = f.atype;
      switch(atype)
      {
          case 2: // int
          {
             if ((c != ' ' ) && (c!= 'i'))
             {
                printf("expected %c in external call but got %d\n",c,atype);
                exit(-1);
             }
             dcArgInt(vm,f.address);
             break;
          }
          case 5:
          {
             if ( (c != ' ' ) && ( c!= 'd'))
             {
                printf("expected double in external call but got %d\n",atype);
                exit(-1);
             }
             double arg_in;
             char* adr = hm->getStart() + f.address;
             memcpy(&arg_in, adr, 8);
             dcArgDouble(vm, arg_in);
             break;
          }
          case 7: // char*
          {
             if ((c !=' ' ) && (c != 'p'))
             {
                printf("expected string or pointer in external call but got %d\n",atype);
                exit(-1);
             } 
                char* adr = hm->getStart() + f.address;
                int len = ((*adr) & 0xff) + (*(adr + 1) << 8);
                char* str = (char*) malloc(len + 1);
                memcpy(str, adr + 2, len);
                str[len] = '\0';
                dcArgPointer(vm, str);
     //           free(str);
                break;
            }
         case 8: // pointer
            {
               if ((c !=' ' ) && (c != 'p'))
               {
                  printf("expected string or pointer in external call but got %d\n",atype);
                  exit(-1);
               }
               dcArgPointer(vm, (void*)f.address);
               break;
            }
          default:
          {
             printf("unexpected type given <%d> but expected <%c> in external call\n", atype,c);
             exit(-1);
          }
      } 

}
