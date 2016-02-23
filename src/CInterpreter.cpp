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
vector<stack_element>* CInterpreter::getStack() {
   return &s;
}

void CInterpreter::start() {
   printf("Starting interpreter...\n" );
   methodmap.clear();
   check_magic_number();
   pc = find_offset();
   printf("PC is now %d\n",pc);
   uint16_t start_ext_proc_table = find_ext_proc_table();
   //
   // fill the methodmap
   //
   for (uint16_t j = 8; j < start_ext_proc_table ; j += 8) {
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
      unsigned long ptr = (buffer[j] & 0xff) + ((buffer[j + 1] & 0xff) << 8) + ((buffer[j + 2] & 0xff) << 16) + ((buffer[j + 3] & 0xff) << 24);
      printf("Pushing external pointer 0x%x\n",ptr);
      j += 4;
      extern_record er;
      string signature = string(buffer + j);
      er.address = ptr;
      er.signature = signature; 
      externs.push_back(er);
      j+= signature.length() + 1;
   } 

#ifdef DEBUG
       printf("end filling methodmap" ); 
#endif
   unsigned i = 0;
   for (; !i;) {
      i = execute_next();
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
   printf("buffer[8] = %d\n",buffer[8]);
   printf("buffer[9] = %d\n",buffer[9]);
   printf("result %d\n",buffer[8] + (buffer[9] >> 8));

   return buffer[8] + (buffer[9] >> 8);
}

/*

 p, b, t: integer; {program-, base-, topstack-registers}
 i: instruction; {instruction register}
 s: array [1..stacksize] of integer; {datastore}
 */
int CInterpreter::execute_next() {
   //
   // f is the opcode
   // l is the 2nd param
   // a is the 3rd param
   //
#ifdef DEBUG
   printf("pc=x" << hex << pc << dec << ": ";
#endif
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
#ifdef DEBUG
   printf("LIT " << l << "," << a;
#endif
      switch (l) {
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
      case 7: // string
         //
         // get some memory
         //
         ptr = hm->allocate(a + 2);
         //
         // copy the string to the heap
         //
         *ptr = a & 0xff;
         *(ptr + 1) = a >> 8;
         memcpy(ptr + 2, buffer + pc, a);
         //
         // put the pointer and the type on the stack
         //
         s[t].atype = 7;
         s[t].address = (uint16_t) (ptr - hm->getStart());
         t++;
         pc += a;
         break;
      case 6: // boolean
         s[t].atype = 6;
         s[t].address = a;
         t++;
         break;
      default:
         puts("unexpected LIT value " + s[t].atype);
      }
      break;
   case 2: // opr
#ifdef DEBUG
   printf("OPR";
#endif

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
#ifdef DEBUG
         printf(" RET l= " << l;
#endif
         // return
         if (tr <= 0) {
            printf("Exiting program...\n" );
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
#ifdef DEBUG
         printf(" UNARY MINUS";
#endif
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
#ifdef DEBUG
         printf(" PLUS, MINUS oR MUL";
#endif
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
            fr3->address = hm->allocate(8) - hm->getStart();
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
            fr3->address = hm->allocate(8) - hm->getStart();
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
            fr3->address = hm->allocate(8) - hm->getStart();
            memcpy(st + fr3->address, &d3, 8);
            s[t - 1] = *fr3;
         } else {
            printf("incompatible types %d and %d\n",fr1.atype,fr2.atype);
         }
         break;
      case 5:
#ifdef DEBUG
         printf(" DIV";
#endif
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
#ifdef DEBUG
         printf(" MOD";
#endif
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
         } else {
            printf( "operation %d: incompatible types %d and %d\n",fr1.atype,fr2.atype);
         }

         break;

      default:
         puts("unexpected A value");
         return -1;
         break;
      }
      break;
   case 3:
#ifdef DEBUG
      printf("LOD " << a << " ";
#endif
      //
      // lod: copy a local variable or parameter on top of the stack
      //
      s[t] = s[b[tb - 1] + a];
      t++;
      break;
   case 4:   // sto: pop a value from the stack and put it in a local variable or parameter
#ifdef DEBUG
   printf("STO " << a << " ";
#endif
      t--;
      s[b[tb - 1] + a] = s[t];
      break;
   case 5:         //cal:
      // parameters should have already been pushed on the stack
      // push the return address on the return stack
      // call the procedure
      //
#ifdef DEBUG
      printf("CAL " << a;
#endif
      r[tr] = pc;
      tr++;
      pc = a;
      break;
   case 6:         // int:
#ifdef DEBUG
   printf("INT " << l << "," << a;
#endif
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
#ifdef DEBUG
   printf("JMP " << a;
#endif
      pc = a;
      break;
   case 8:         // jpc - jump when false
#ifdef DEBUG
   printf("JPC " << a;
#endif
      fr1 = s[t - 1];
      if (fr1.atype != 6) {
         puts("JPC value is not boolean");
      }
      if (fr1.address == 0) {
         pc = a;
      }
      t--;
      break;
   case 9: // print
#ifdef DEBUG
   printf("PRINT " << a;
#endif
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
         printf("A fancy object\n" );
      } else {
         printf( "Cannot print something of type %d\n", fr1.atype );
      }
      break;

   case 10: // external function call
#ifdef DEBUG
   printf("EXTCALL " << a;
#endif
      // parameters should have already been pushed on the stack
      //
      //
      // call the procedure
      //
      call_external(l);
      break;
   case 11: // object creation
#ifdef DEBUG
   printf("OBJCREATE " << l<<"," << a;
#endif

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
#ifdef DEBUG
      printf("METHODCALL " << l << " ";
#endif
      // method call, the object is already on the stack.
      //
      // lookup the method by object type and name
      // call the method
      //
      // l is the method number
      // a is unused
      //
      // figure out what class type is on top of the stack
      //
      t--;
      if (s[t].atype != 8) {
         puts("Performed a method call on a nonfancy object");
      }
      ptr = hm->getStart() + s[t].address;
      classnum = (*ptr & 0xff) + (*(ptr + 1) << 8);
#ifdef DEBUG
                printf("classnum = " << classnum << " ";
#endif
      //
      // this creates a new block with depth for local variables and parameters
      //
      b[tb] = t - methodmap[l][classnum][1];
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
#ifdef DEBUG
      printf("LODI " << l << " ";
#endif
      // access an instance variable and put it on the stack
      //
      // l is the instance variable
      // a is unused
      //
      // first get the this pointer
      // then calculate the address of the inst. variable
      adr = s[b[tb - 1]].address + 3 * l + 3;
      //
      // put the value on the stack
      //
      s[t].atype = *(hm->getStart() + adr) & 0xff;
      s[t].address = (*(hm->getStart() + adr + 1) & 0xff)
            + (*(hm->getStart() + adr + 2) << 8);
      t++;
      break;
   case 14:
#ifdef DEBUG
      printf("STOI " << l << " ";
#endif
      // store a value inside an inst. variable
      //
      // l is the instance variable
      // a is unused
      //
      // first get the this pointer
      // then calculate the address of the inst. variable
      //
      adr = s[b[tb - 1]].address + 3 * l + 3;
      //
      // store the value on the heap
      //
      *(hm->getStart() + adr) = s[t].atype;
      *(hm->getStart() + adr + 1) = s[t].address & 0xff;
      *(hm->getStart() + adr + 2) = s[t].address >> 8;
      t--;
      break;
   default:
      puts("unexpected F value");
      break;
   }
   //
   // print the stack
   //
#ifdef DEBUG
   printf("      stack: ";
   for (uint16_t i = 0; i < t; i++) {
      char* adr;
      printf("[";
      switch (s[i].atype) {
         case 2:
            printf(s[i].address;
            break;
         case 5:
            adr = hm->getStart() + s[i].address;
            double d;
            memcpy(&d, adr, 8);
            printf(d;
            break;
         case 7: // string
            adr = hm->getStart() + s[i].address;
            print_a_string(adr,false);
            printf("]";
            break;
         default:
            printf("?" << s[i].atype;
            break;
      }
      printf("]";
   }
   printf("      bstack: ";
   for (uint16_t i = 0; i < tb; i++) {
      printf(b[i] << " ";
   }
   printf("      rstack: ";
   for (uint16_t i = 0; i < tr; i++) {
      printf("x" << hex << r[i] << dec << " ";
   }
   printf(endl;
#endif
   return 0;
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
void CInterpreter::call_external(short unsigned int function_number) {
   if (function_number > externs.size())
   {
        printf("illegal function number %d highest function number is %d\n",function_number,externs.size());
        exit(-1);
   }
   extern_record e = externs[function_number-1];
   void* sym = (void*)(e.address);
   string signature = e.signature;
   size_t pos = signature.find(")");
   string ingoing =  signature.substr(0,pos);
   string outgoing = signature.substr(pos+1); 
   DCCallVM* vm = dcNewCallVM(4096);
   dcMode(vm, DC_CALL_C_DEFAULT);
   dcReset(vm);
   //
   // loop over the signature
   //
   for(char& c : ingoing) {
      stack_element f = s[t - 1];
      uint16_t atype = f.atype;
      switch (c)
      {
          case 'D':
             if (atype != 5)
             {
                printf("expected double in external call but got %d\n",atype);
                exit(-1);
             }
             double arg_in;
             char* adr;
             adr = hm->getStart() + f.address;
             memcpy(&arg_in, adr, 8);
             dcArgDouble(vm, arg_in);
             break;
          default:
             puts("unexpected type in external call");
             exit(-1);
      } 
   } 

/*

      case 7: // string
         adr = hm->getStart() + f.address;
         memcpy(&arg_in, adr, 8);
         len = ((*adr) & 0xff) + (*(adr + 1) << 8);
         str = (char*) malloc(len + 1);
         memcpy(str, adr + 2, len);
         str[len] = '\0';
         dcArgChar(vm, *str);
         free(str);
         break;

*/

   char c = outgoing[0];
   switch (c)
   {
      case 'D':
     {
         double r = dcCallDouble(vm, sym);
         //
         // put the result on the stack
         //
         char* ptr = hm->allocate(8);
         memcpy(ptr, &r, 8);
         t = b[tb] + 2;
         tb--;
         s[t].atype = 5;
         s[t].address = (short uint16_t) (ptr - hm->getStart());
         t++;
         break;
      }
      default:
         puts("unexpected return type in external call");
         exit(-1);

   }
   dcFree(vm);
}
