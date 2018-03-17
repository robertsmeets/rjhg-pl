/*
 * CInterpreter.c
 *
 *  Created on: Dec 11, 2014
 *      Author: robert
 */

#include "CInterpreter.h"
#include "stdint.h"


void CI_init(char* a_buffer) {
   buffer=a_buffer;
   pc = 0; // program counter
   t = 0;   // is the top of the stack s
   tr = 0;  // is the top of the stack r
   tb = 0;  // is the top of the stack b
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
   fptrs[OPCODE_PLS][2][2] = (iiptr) (&func_plus_ii);
   fptrs[OPCODE_PLS][2][5] = (iiptr) (&func_plus_id);
   fptrs[OPCODE_PLS][5][2] = (iiptr) (&func_plus_di);
   fptrs[OPCODE_PLS][5][5] = (iiptr) (&func_plus_dd);
   //
   // fill up for OPR 3 (MINUS);
   //
   fptrs[OPCODE_MIN][2][2] = (iiptr) (&func_minus_ii);
   fptrs[OPCODE_MIN][2][5] = (iiptr) (&func_minus_id);
   fptrs[OPCODE_MIN][5][2] = (iiptr) (&func_minus_di);
   fptrs[OPCODE_MIN][5][5] = (iiptr) (&func_minus_dd);
   //
   // fill up for OPR 4 (MUL);
   //
   fptrs[OPCODE_MUL][2][2] = (iiptr) (&func_mul_ii);
   fptrs[OPCODE_MUL][2][5] = (iiptr) (&func_mul_id);
   fptrs[OPCODE_MUL][5][2] = (iiptr) (&func_mul_di);
   fptrs[OPCODE_MUL][5][5] = (iiptr) (&func_mul_dd);
   //
   // fill up for OPR 8..13
   //
   fptrs[OPR_EQ][0][0] = (iiptr) (&func_eq_nn);
   fptrs[OPR_EQ][2][2] = (iiptr) (&func_eq_ii);
   fptrs[OPR_EQ][2][5] = (iiptr) (&func_eq_id);
   fptrs[OPR_EQ][5][2] = (iiptr) (&func_eq_di);
   fptrs[OPR_EQ][5][5] = (iiptr) (&func_eq_dd);
   fptrs[OPR_EQ][0][9] = (iiptr) (&func_eq_no);
   fptrs[OPR_EQ][9][0] = (iiptr) (&func_eq_on);

   fptrs[OPR_NE][2][2] = (iiptr) (&func_ne_nn);
   fptrs[OPR_NE][2][2] = (iiptr) (&func_ne_ii);
   fptrs[OPR_NE][2][5] = (iiptr) (&func_ne_id);
   fptrs[OPR_NE][5][2] = (iiptr) (&func_ne_di);
   fptrs[OPR_NE][5][5] = (iiptr) (&func_ne_dd);

   fptrs[OPR_LT][2][2] = (iiptr) (&func_lt_ii);
   fptrs[OPR_LT][2][5] = (iiptr) (&func_lt_id);
   fptrs[OPR_LT][5][2] = (iiptr) (&func_lt_di);
   fptrs[OPR_LT][5][5] = (iiptr) (&func_lt_dd);

   fptrs[OPR_GE][2][2] = (iiptr) (&func_ge_ii);
   fptrs[OPR_GE][2][5] = (iiptr) (&func_ge_id);
   fptrs[OPR_GE][5][2] = (iiptr) (&func_ge_di);
   fptrs[OPR_GE][5][5] = (iiptr) (&func_ge_dd);

   fptrs[OPR_GT][2][2] = (iiptr) (&func_gt_ii);
   fptrs[OPR_GT][2][5] = (iiptr) (&func_gt_id);
   fptrs[OPR_GT][5][2] = (iiptr) (&func_gt_di);
   fptrs[OPR_GT][5][5] = (iiptr) (&func_gt_dd);

   fptrs[OPR_LE][2][2] = (iiptr) (&func_le_ii);
   fptrs[OPR_LE][2][5] = (iiptr) (&func_le_id);
   fptrs[OPR_LE][5][2] = (iiptr) (&func_le_di);
   fptrs[OPR_LE][5][5] = (iiptr) (&func_le_dd);

}

int CI_getStackDepth()
{
   return t;
}

struct stack_element* CI_getStack() {
   return s;
}

void CI_start(bool indebug) {
   debug=indebug;
   if (debug) {printf("Starting interpreter...\n" );}
   methodmap = create_hash(1);
   CI_check_magic_number();
   pc = CI_find_offset();
   if (debug){ printf("PC is now 0x%x\n",pc);}
   uint16_t start_ext_proc_table = CI_find_ext_proc_table();
   //
   // fill the methodmap
   //
   for (uint16_t j = 10; j < start_ext_proc_table ; j += 8) {
      uint16_t classnum = (buffer[j] & 0xff) + ((buffer[j + 1] & 0xff) << 8);
      uint16_t methodnum = (buffer[j + 2] & 0xff) + ((buffer[j + 3] & 0xff) << 8);
      uint16_t address = (buffer[j + 4] & 0xff) + ((buffer[j + 5] & 0xff) << 8);
      uint16_t num_params = buffer[j + 6];
      uint16_t num_local_vars = buffer[j + 7];
      void* sstr1; 
      //
      // check if this entry already exists
      //
      get_ptr_by_int(methodmap,methodnum,&sstr1); 
      if (sstr1 == NULL) {
         //
         // was not found
         //
         jwHashTable* map2 = create_hash(100);
         uint16_t* ar = (uint16_t*)GC_MALLOC(3* sizeof(uint16_t));
         ar[0] = address;
         ar[1] = num_params;
         ar[2] = num_local_vars;
         if(debug) {printf("adding %d %d %x %d %d\n",classnum, methodnum, address,num_params,num_local_vars);}
         if(debug) {printf("ar = %x\n",ar);}
         add_ptr_by_int(map2,classnum,ar);
         add_ptr_by_int(methodmap,methodnum,(void*)map2);
      }
      else
      {  
         //
         // was found. See if the class has this method
         //
         uint16_t* ar;
         get_ptr_by_int((jwHashTable*)sstr1,classnum,(void**)&ar);
         if (ar==NULL)
         {
            //
            // class does not have this method. Add it
            //
            if(debug) {printf("adding %d %d %x %d %d\n",classnum, methodnum, address,num_params,num_local_vars);}
            uint16_t* bar = (uint16_t*)GC_MALLOC(3* sizeof(uint16_t));
            bar[0] = address;
            bar[1] = num_params;
            bar[2] = num_local_vars;
            add_ptr_by_int((jwHashTable*)sstr1,classnum,(void*)bar);
            if(debug) {printf("bar = %x\n",bar);}
         }
         else
         {
            if(debug) {printf("NOT adding %d %d %x %d %d\n",classnum, methodnum, address,num_params,num_local_vars);}
         }
      }
   }
   //
   // get the external symbols
   //
   extern_count=0;
   uint16_t j = start_ext_proc_table;
   while (j < pc)
   {
      void* ptr;
      memcpy(&ptr,buffer + j,8); 
      j += 8;
      struct extern_record er;
      int len = strlen(buffer + j);
      char* signature = GC_MALLOC(len+1);
      strncpy(signature,buffer+j,len+1);
      er.address = (long long unsigned int)ptr;
      er.signature = signature; 
      externs[extern_count]=er;
      extern_count++;
      j+= len + 1;
   } 
   unsigned i = 0;
   for (; !i;) {
      i = CI_execute_next();
   }
}

void CI_check_magic_number() {
   if (!(buffer[0] == 'R' && buffer[1] == 'J' && buffer[2] == 'H'
         && buffer[3] == 'G' && buffer[4] == 'P' && buffer[5] == 'L')) {
      puts("Magic number does not match, invalid bytecode");
   }
}

uint16_t CI_find_offset() {
   uint16_t* uptr = (uint16_t*) (buffer + 6);
   return *uptr;
}

uint16_t CI_find_ext_proc_table() {
   uint16_t* uptr = (uint16_t*) (buffer + 8);
   return *uptr;
}

/*

 p, b, t: integer; {program-, base-, topstack-registers}
 i: instruction; {instruction register}
 s: array [1..stacksize] of integer; {datastore}
 */
int CI_execute_next() {
   //
   // f is the opcode
   // l is the 2nd param
   // a is the 3rd param
   //
   if (debug) { printf("pc=0x%x: ", pc ); }
   unsigned short int f = *((char*) buffer + pc) & 0xff;
   pc++;
   unsigned short int l = 0;
   unsigned short int a = 0;
   if ((f != OPCODE_DRP) && (f != OPCODE_PRT)) // not for DROP and PRINT
   {
      //
      // using little endian
      //
      char* lptr = (char*) buffer + pc;
      l = (*lptr & 0xff) + (*(lptr + 1) << 8);
      pc += 2;
      if (f != OPCODE_RET) // not for RET
      {
         lptr = (char*) buffer + pc;
         a = (*lptr & 0xff) + (*(lptr + 1) << 8);
         pc += 2;
      }
   }
   struct stack_element temp;
   //
   // opcode definitions
   //
   // stack_element fr1;
   // stack_element fr2;
   char* adr;
   uint16_t classnum;
   switch (f) {
   case OPCODE_LIT:   // lit: Literal value, to be pushed on the stack
      if (debug) { printf("LIT %d,%d", l , a); }
      switch (l) {
      case TYPE_NULL: // null
         s[t].atype=0;
         s[t].address = 0;
         t++;
         break;
      case TYPE_INT : // Int
         s[t].atype = 2;
         s[t].address = a;
         t++;
         break;
      case TYPE_FLOAT: // float
         {
         char* ptr = (char*) GC_MALLOC(a);
         memcpy(ptr, buffer + pc, a);
         //
         // put the pointer and the type on the stack
         //
         s[t].atype = 5;
         s[t].address = (unsigned long long int) ptr;
         t++;
         pc += a;
         break;}
      case TYPE_BOOLEAN: // boolean
         s[t].atype = 6;
         s[t].address = a;
         t++;
         break;
      case TYPE_STRING: // string
      {
         //
         // get some memory
         //
         char* tmp = (char*)GC_MALLOC(a + 2);
         //
         // copy the string to the heap
         //
         *tmp = a & 0xff;
	 *(tmp + 1) = (a >> 8) & 0xff;
         memcpy(tmp + 2, buffer + pc, a);
         //
         // put the pointer and the type on the stack
         //
         s[t].atype = 7;
         s[t].address = (unsigned long long int) tmp;
         t++;
         pc += a;
         break;
      }
      default:
         puts("unexpected LIT value " + s[t].atype);
      }
      break;
    case OPCODE_PLS:
    case OPCODE_MIN:
    case OPCODE_MUL:
         {
         if (debug) { printf(" PLUS, MINUS or MUL"); }
      iiptr aiiptr;
      idptr aidptr;
      diptr adiptr;
      ddptr addptr;

      biiptr abiiptr;
      bidptr abidptr;
      bdiptr abdiptr;
      bddptr abddptr;

         t--;
         struct stack_element fr1 = s[t - 1];
         struct stack_element fr2 = s[t];
         if ((fr1.atype == TYPE_INT) && (fr2.atype == TYPE_INT)) {
            //
            // operation on two integers
            //
            aiiptr = fptrs[f][fr1.atype][fr2.atype];
            fr1.address = (*aiiptr)(fr1.address, fr2.address);
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_STRING) && (fr2.atype == TYPE_STRING)) {
            //
            // add two strings
            //
            char * ptr1 = (char*) fr1.address;
            char * ptr2 = (char*) fr2.address;
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            uint16_t len2 = ((*ptr2) & 0xff) + (*(ptr2 + 1) << 8);
            uint16_t newlen = len1 + len2;
            char* ptr = (char*)GC_MALLOC(newlen + 2);
            *ptr = newlen & 0xff;
            *(ptr + 1) = newlen >> 8;
            memcpy(ptr + 2, ptr1 + 2, len1);
            memcpy(ptr + len1 + 2, ptr2 + 2, len2);
            fr1.address = (long long unsigned int) (ptr );
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_STRING) && (fr2.atype == TYPE_INT)) {
            //
            // add a string and an integer
            //
            char* ptr1 = (char*) fr1.address;
            char str[15];
            snprintf(str, 15, "%llu", fr2.address);
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            uint16_t len2 = strlen(str);
            uint16_t newlen = len1 + len2;
            char* ptr = (char*)GC_MALLOC(newlen + 2);
            *ptr = newlen & 0xff;
            *(ptr + 1) = newlen >> 8;
            memcpy(ptr + 2, ptr1 + 2, len1);
            memcpy(ptr + len1 + 2, &str, len2);
            fr1.address = (long long unsigned int) (ptr );
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_STRING) && (fr2.atype == TYPE_NULL)) {
            //
            // add a string and a null
            //
            char* ptr1 = (char*) fr1.address;
            char str[5];
            snprintf(str, 4, "null");
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            uint16_t len2 = strlen(str);
            uint16_t newlen = len1 + len2;
            char* ptr = (char*)GC_MALLOC(newlen + 2);
            *ptr = newlen & 0xff;
            *(ptr + 1) = newlen >> 8;
            memcpy(ptr + 2, ptr1 + 2, len1);
            memcpy(ptr + len1 + 2, &str, len2);
            fr1.address = (long long unsigned int) (ptr );
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_STRING) && (fr2.atype == TYPE_OBJ)) {
            //
            // add a string and an object
            //
            char* ptr1 = (char*) fr1.address;
            char str[7];
            snprintf(str, 6, "object");
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            uint16_t len2 = strlen(str);
            uint16_t newlen = len1 + len2;
            char* ptr = (char*)GC_MALLOC(newlen + 2);
            *ptr = newlen & 0xff;
            *(ptr + 1) = newlen >> 8;
            memcpy(ptr + 2, ptr1 + 2, len1);
            memcpy(ptr + len1 + 2, &str, len2);
            fr1.address = (long long unsigned int) (ptr );
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_INT) && (fr2.atype == TYPE_FLOAT)) {
            //
            // integer plus float
            //
            double d2;
            memcpy(&d2, (void*) fr2.address, 8);
            aidptr = (idptr) (fptrs[f][fr1.atype][fr2.atype]);
            double d3 = (*aidptr)(fr1.address, d2);
            struct stack_element se;
            struct stack_element* fr3 = &se; 
            fr3->atype = 5;
            char* tmp = (char*)GC_MALLOC(8);
            fr3->address = (long long unsigned int) tmp ;
            memcpy((void*) (fr3->address), &d3, 8);
            s[t - 1] = *fr3;
         } else if ((fr1.atype == TYPE_FLOAT) && (fr2.atype == TYPE_INT)) {
            //
            // float plus integer
            //
            double d1;
            memcpy(&d1, (void*) (fr1.address), 8);
            adiptr = (diptr) (fptrs[f][fr1.atype][fr2.atype]);
            double d3 = (*adiptr)(d1, fr2.address);
            struct stack_element se;
            struct stack_element* fr3 = &se;
            fr3->atype = 5;
            char* tmp = (char*)GC_MALLOC(8);
            fr3->address = (long long unsigned int)tmp ;
            memcpy((void*) (fr3->address), &d3, 8);
            s[t - 1] = *fr3;
         } else if ((fr1.atype == TYPE_FLOAT) && (fr1.atype == TYPE_FLOAT)) {
            //
            // both floats
            //
            //
            // copy both floats to temp variables d1 and d2
            //
            double d1;
            double d2;
            memcpy(&d1, (void*)(fr1.address), 8);
            memcpy(&d2, (void*)(fr2.address), 8);
            //
            // find the function
            //
            addptr = (ddptr) (fptrs[f][fr1.atype][fr2.atype]);
            //
            // perform the operation
            //
            double d3 = (*addptr)(d1, d2);
            //
            //
            struct stack_element se;
            struct stack_element* fr3 = &se;
            fr3->atype = 5;
            char* tmp = (char*)GC_MALLOC(8);
            fr3->address = (long long unsigned int) tmp ;
            memcpy((void*)(fr3->address), &d3, 8);
            s[t - 1] = *fr3;
         } else {
            printf("operation %d incompatible types %d and %d\n",a,fr1.atype,fr2.atype);
            exit(-1);
         }
         break; }
      case OPCODE_UNA: {if (debug) { printf(" UNARY MINUS"); } struct stack_element fr1 = s[t - 1]; if (fr1.atype != 2) { puts("type must be integer"); } fr1.address = -fr1.address; s[t - 1] = fr1; break;}
      case OPCODE_DIV:
         {
         if (debug) { printf(" DIV"); }
         t--;
         struct stack_element fr1 = s[t - 1];
         struct stack_element fr2 = s[t];
         if ((fr1.atype != 2) || (fr2.atype != 2)) {
            puts("division both types must be integer");
         }
         fr1.atype = 2;
         fr1.address = fr1.address / fr2.address;
         s[t - 1] = fr1;
         break; }
      case OPCODE_MOD:
         {if (debug) { printf(" MOD"); }
         t--;
         struct stack_element fr1 = s[t - 1];
         struct stack_element fr2 = s[t];
         if ((fr1.atype != 2) || (fr2.atype != 2)) {
            puts("modulo both types must be integer");
         }
         fr1.atype = 2;
         fr1.address = fr1.address % fr2.address;
         s[t - 1] = fr1;
         break;}
   case OPCODE_OPR: // opr
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
      case OPR_EQ:
      case OPR_NE:
      case OPR_LT:
      case OPR_GE:
      case OPR_GT:
      case OPR_LE:
         {t--;
         struct stack_element fr1 = s[t - 1];
         struct stack_element fr2 = s[t];
         if ((fr1.atype == TYPE_INT) && (fr2.atype == TYPE_INT)) {
            //
            // operation on two integers
            //
            abiiptr = (biiptr) fptrs[a][fr1.atype][fr2.atype];
            bool eq = (*abiiptr)(fr1.address, fr2.address);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_INT) && (fr2.atype == TYPE_FLOAT)) {
            //
            // integer plus float
            //
            double d2;
            memcpy(&d2, (void*)(fr2.address), 8);
            abidptr = (bidptr) (fptrs[a][fr1.atype][fr2.atype]);
            bool eq = (*abidptr)(fr1.address, d2);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_FLOAT) && (fr2.atype == TYPE_INT)) {
            //
            // float plus integer
            //
            double d1;
            memcpy(&d1, (void*)(fr1.address), 8);
            abdiptr = (bdiptr) (fptrs[a][fr1.atype][fr2.atype]);
            bool eq = (*abdiptr)(d1, fr2.address);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } else if ((fr1.atype == TYPE_FLOAT) && (fr1.atype == TYPE_FLOAT)) {
            //
            // both floats
            //
            double d1,d2;
            memcpy(&d1, (void*)(fr1.address), 8);
            memcpy(&d2, (void*)(fr2.address), 8);
            abddptr = (bddptr) (fptrs[a][fr1.atype][fr2.atype]);
            bool eq = (*abddptr)(d1, d2);
            fr1.atype = 6;
            fr1.address = eq;
            s[t - 1] = fr1;
         } 
          else if ((fr1.atype == TYPE_NULL) && (fr2.atype == TYPE_NULL)) {
            //
            // null vs null
            //
            bool eq = true;
            fr1.atype=6;
            if (a==9)
            {
                eq = !eq;
            }
            fr1.address=eq;
            s[t-1]= fr1;
          }
          else if ((fr1.atype == TYPE_NULL) && (fr2.atype == TYPE_PTR)) {
            //
            // null vs pointer 
            //
            bool eq = (fr2.address == 0);
            fr1.atype=6;
            fr1.address=eq;
            s[t-1]= fr1;
          }
          else if ((fr1.atype == TYPE_PTR) && (fr2.atype == TYPE_NULL)) {
            //
            // pointer vs null
            //
            bool eq = (fr1.address == 0);
            fr1.atype=6;
            fr1.address=eq;
            s[t-1]= fr1;
          }
          else if (((fr1.atype == TYPE_OBJ)||(fr1.atype==TYPE_STRING)) && (fr2.atype == TYPE_NULL)) {
            //
            // pointer vs null
            //
            bool eq = false;
            fr1.atype=6;
            if (a==9)
            {
                eq = !eq;
            }
            fr1.address=eq;
            s[t-1]= fr1;
          }
          else if ((fr1.atype == TYPE_NULL) && ((fr2.atype == TYPE_OBJ) || (fr2.atype == TYPE_STRING))) {
            //
            // pointer vs null
            //
            bool eq = false;
            fr1.atype=6;
            if (a==9)
            {
                eq = !eq;
            }
            fr1.address=eq;
            s[t-1]= fr1;}
          else if ((fr1.atype == TYPE_STRING) && (fr2.atype == TYPE_STRING)) {
            //
            // string vs string
            //
            char * ptr1 = (char*) fr1.address;
            uint16_t len1 = ((*ptr1) & 0xff) + (*(ptr1 + 1) << 8);
            char * ptr2 = (char*) fr2.address;
            uint16_t len2 = ((*ptr2) & 0xff) + (*(ptr2 + 1) << 8);
            bool eq;
            if (len1 == len2)
            {
               eq = (strncmp(ptr1+2,ptr2+2,len1) == 0);
            }
            else
            {
               eq = false;
            }
            if (a==9)
            {
               eq=!eq;
            }
            fr1.atype=6;
            fr1.address=eq;
            s[t-1]= fr1;
         }
         else {
            printf( "operation %d: incompatible types %d and %d\n",a,fr1.atype,fr2.atype);
            exit(-1);
         }
         break; }
      case OPR_I:
         {
            //
            // Array index operation
            //
            int atype = s[t-1].atype;
            if (atype != TYPE_INT) { printf("index is not an integer but type %d\n",atype); exit(-1);}
            int index = s[t-1].address;
            atype=s[t-2].atype;
            if (atype == TYPE_ARRAY)
            {char* ptr = (char*)(s[t-2].address);
            //
            // byte 0 and 1 are the actual length
            // byte 2 and 3 are the claimed length
            //
            int actual = ((*ptr) & 0xff) + ((*(ptr + 1))  << 8) ;
            if (index > actual) { printf("index out of bounds: %d array size = %d\n",index,actual); exit(-1);}
            //
            // get the spaceptr
            //
            char* mptr = ptr+4;
            char** aptr = (char**) mptr;
            char* spaceptr = *aptr;
            //
            // get the value
            //
            char* nptr = spaceptr + index * 8 ;
            t--;
            s[t-1].atype = *nptr ;
            char** xptr = (char**)(nptr+1);
            char* zptr = (char*) ((long long unsigned int)(*xptr) & 0xffffffffffff);
            s[t-1].address = (long long unsigned int) zptr;
            }
            else
            {
              if (atype == TYPE_STRING)
              {
                 //
                 // string index operation
                 //
                 char* ptr = (char*)(s[t-2].address);
                 char* cptr = ptr+2+index;
                  t--;
                  s[t-1].atype=TYPE_INT;
                  s[t-1].address=*cptr;
               }else
               { printf("indexed type is not an array but type %d\n",atype); exit(-1);}
            }
            break;
         }
	 case OPR_NOT: // NOT
	 {
	 if (debug) { printf("NOT"); }
         struct stack_element fr1 = s[t - 1];
         if (fr1.atype != TYPE_BOOLEAN) {
            printf("NOT: type must be boolean");
	    exit(-1);
         }
         fr1.address = !fr1.address;
         s[t - 1] = fr1;
         break;
	 }
	 case OPR_AND: // AND
	 {
            t--;
            struct stack_element fr1 = s[t - 1];
            struct stack_element fr2 = s[t];
         if ((fr1.atype != TYPE_BOOLEAN) || (fr2.atype != TYPE_BOOLEAN)) {
            printf("AND: both types must be boolean");
	    exit(-1);
            }
            fr1.address=fr1.address && fr2.address;
         s[t - 1] = fr1;
          break;
	 }
	 case OPR_OR: // OR
	 {
            t--;
            struct stack_element fr1 = s[t - 1];
            struct stack_element fr2 = s[t];
         if ((fr1.atype != TYPE_BOOLEAN) || (fr2.atype != TYPE_BOOLEAN)) {
            printf("OR: both types must be boolean");
	    exit(-1);
            }
            fr1.address=fr1.address || fr2.address;
         s[t - 1] = fr1;
          break;
	 }
      default:
         printf("unexpected A value %d",a);
         return -1;
         break;
      }
      break;
   case OPCODE_LOD:
      if (debug) { printf("LOD %d ",a); }
      //
      // lod: copy a local variable or parameter on top of the stack
      //
      s[t] = s[b[tb - 1] + a];
      t++;
      break;
   case OPCODE_STO:   // sto: pop a value from the stack and put it in a local variable or parameter
      if (debug) { printf("STO %d " ,a); }
      t--;
      s[b[tb - 1] + a] = s[t];
      break;
   case OPCODE_CAL:         //cal:
      // parameters should have already been pushed on the stack
      // push the return address on the return stack
      // call the procedure
      //
      if (debug) { printf("CAL %d ",a); }
      r[tr] = pc;
      tr++;
      pc = a;
      break;
   case OPCODE_INT:         // int:
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
   case OPCODE_JMP:         // jmp
      if (debug) { printf("JMP %d " , a); }
      pc = a;
      break;
   case OPCODE_JPC:         // jpc - jump when true
      {if (debug) { printf("JPC %d " , a); }
      struct stack_element fr1 = s[t - 1];
      if (fr1.atype != 6) {
         puts("JPC value is not boolean");
      }
      if (fr1.address != 0) {
         pc = a;
      }
      t--;
      break;}
   case OPCODE_JPF:          // jpf: jump when false
      {if (debug) { printf("JPF %d " , a); }
      struct stack_element fr1 = s[t - 1];
      if (fr1.atype != 6) {
         puts("JPF value is not boolean");
      }
      if (fr1.address == 0) {
         pc = a;
      }
      t--;
      break;}
   case OPCODE_PRT: // print
      {
      t--;
      struct stack_element fr1 = s[t];
      if (fr1.atype == TYPE_NULL)
      {
          printf("NULL\n");
      }
      else if (fr1.atype == TYPE_STRING) {
         char* ptr = (char*) (fr1.address);
         CI_print_a_string(ptr,true);
      } else if (fr1.atype == TYPE_FLOAT) {
         //
         // float
         //
         char* ptr = (char*) (fr1.address);
         double d1;
         memcpy(&d1, ptr, 8);
         printf("%f\n",d1 );
      } else if (fr1.atype == TYPE_INT) {
         printf("%llu\n",fr1.address );
      } else if (fr1.atype == TYPE_BOOLEAN) { // boolean
         if (fr1.address) {
            printf("true\n" );
         } else {
            printf("false\n" );
         }
      } else if (fr1.atype == TYPE_PTR) {
         printf("A Pointer\n" );
      } else if (fr1.atype == TYPE_OBJ) {
         printf("A Fancy Object\n" );
      } else {
         printf( "Cannot print something of type %d\n", fr1.atype );
      }
      break;
      }
   case OPCODE_EXT: // external function call
      if (debug) { printf("EXTCALL %d",  a); }
      // parameters should have already been pushed on the stack
      //
      //
      // call the procedure
      //
      CI_call_external(l,a);
      break;
   case OPCODE_OBJ: // object creation
      if (debug) { printf("OBJCREATE %d,%d " ,l,a); }
      if (l == 1)
      {
         //
         // this creates a array of size x
         //
         // 
         // initial claimed size is 5 elements
         // byte 0 and 1 are the actual length
         // byte 2 and 3 are the claimed length
         // byte 4 to 11 are the pointer to the allocated memory
         //
         int claimed = 5;
         char* ptr = (char*)GC_MALLOC(12);
         *ptr = 0;
         *(ptr + 1) = 0;
         *(ptr + 2) = claimed;
         *(ptr + 3) = 0;
         char* nptr = ptr + 4;
         char** vptr = (char**)nptr;
         char* nnptr = (char*)GC_MALLOC(8 * claimed); 
         *vptr = nnptr; 
         //
         // leave the new object on the stack
         //
         s[t].atype = TYPE_ARRAY; 
         s[t].address = (long long unsigned int) ptr;
         t++;
      }
      else
      {
         //
         // l contains the classnum
         // a contains the number of instance variables
         //
         char* ptr = (char*)GC_MALLOC(5 * a + 3);
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
         s[t].atype = TYPE_OBJ; // Object
         s[t].address = (long long unsigned int) ptr ;
         t++;
      }
      break;
   case OPCODE_MCL:{
      if (debug) printf("METHODCALL %d %d ",l,a);
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
      int xtype = s[t-1].atype;
      if (debug) printf("xtype is %d\n",xtype);
      if ((xtype != TYPE_OBJ) && (xtype != TYPE_ARRAY) && (xtype != TYPE_STRING)) {
         printf("Performed a method call on a non-object");
         return -1;
      }
      char* ptr = (char*)(s[t-1].address);
      if (xtype == TYPE_ARRAY)
      {
        if (l==1)
        {
           // 
           // array.add() method
           //
           array_add(ptr,s,&t,debug);
        }
        else if (l==2)
        {
           // 
           // array.set() method
           //
           array_set(ptr,s,&t,debug);
        }
        else
        {
           printf("unsupported l = %d\n",l);
           exit(-1);
        }
      }
      else
      {
         if  (xtype == TYPE_STRING)
         {
            if (l==3)
            {
              string_size(ptr,s,&t,debug);
            }
            else
            {
              printf("unsupported l = %d\n",l);
              exit(-1);
            }
         }
         else
         {
            //
            // figure out the classnum
            //
            uint16_t* uptr = (uint16_t*) ptr;
            uint16_t classnum = *uptr;
            if (classnum == 0)
            {
                   printf("cannot call a method on class 0\n");
                   exit(-1);
            }
            if(debug)printf("Calling method %d on class %d\n",l,classnum);
            //
            // this creates a new block with depth for local variables and parameters
            //
            //
            // some checks here to make sure this method exists
            // 
            jwHashTable* thing;
            get_ptr_by_int(methodmap,l,(void**)&thing);
            if (thing==NULL) { printf("class does not have method (one)\n"); exit(-1); } 
            uint16_t* cl;
            get_ptr_by_int(thing,classnum,(void**)&cl);
            if (cl==NULL) { printf("class does not have method (two)\n"); exit(-1); } 
            if(debug){
                 printf("-------------- Retrieved  methodnum =  %d classnum =  %d ptr= %x\n",l,classnum,cl);
                 printf("cl[0] = %x\n",cl[0]);
                 printf("cl[1] = %d\n",cl[1]);
                 printf("cl[2] = %d\n",cl[2]);
            }
            b[tb] = t - a - 1;
            tb++;
            //
            // add room for the this pointer and the local vars
            //
            t += cl[2];
            //
            // add the program counter on the return stack
            //
            r[tr] = pc;
            tr++;
            pc = cl[0];
            }
         }
         break;}
      case OPCODE_LDI: {
         if (debug) { printf("LDI %d %d", l, a); }
         // access an instance variable and put it on the stack
         //
         // l is the instance variable
         // a is the number of parameters
         //
         // first get the this pointer
         // then calculate the address of the inst. variable
         // But what is the offset of the this pointer?
         //
         unsigned int offset_self = b[tb-1] + a;
         adr = (char*) (s[offset_self].address + 5 * l +3);
         //
         // put the value on the stack
         //
         s[t].atype = *((char*)adr) & 0xff;
         uint32_t* uptr = (uint32_t*)(adr+1); 
         s[t].address = *uptr;
         t++;
         break; }
      case OPCODE_STI:{
         if (debug) { printf("STI %d %d", l,a ); }
         // store a value inside an inst. variable
         //
      // l is the instance variable
      // a is the number of parameters
      //
      // first get the this pointer
      // then calculate the address of the inst. variable
      //
      char* selfptr =  (char*) (s[b[tb - 1] + a].address );
      adr = (char*) (selfptr+ 5 * l + 3);
      //
      // store the value on the heap
      //
      *((char*) adr) = s[t-1].atype;
      uint32_t* uptr = (uint32_t*)(adr+1);
      *uptr = s[t-1].address; 
      t--;
      break;}
   case OPCODE_DRP:
      if (debug) { printf("DROP"); }
      //
      // drop the top value of the stack
      //
      t--;
      break;
   case OPCODE_SLF:{
      // 
      // self
      //
      if (debug){printf("SELF");}
      adr = (char*) (s[b[tb - 1] + a-1].address);
      //
      // put the value on the stack
      //
      s[t].atype = TYPE_OBJ;
      s[t].address = (unsigned long long int)adr;
      t++;
      break;
     }
      case OPCODE_RET: { // RET
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
   }
   default:
      printf("unexpected F value <0x%x> at PC <0x%x>\n",f,pc);
      exit(-1);
      break;
   }
   CI_print_stack();
   return 0;
}


void CI_print_stack()
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
         case TYPE_NULL:
            printf("NULL");
            break;
         case TYPE_ARRAY:
            printf("array %p",(void*)s[i].address);
            break;
         case TYPE_INT:
            printf("%llu",s[i].address);
            break;
         case TYPE_FLOAT:
            adr = (char*)(s[i].address);
            double d;
            memcpy(&d, adr, 8);
            printf("%f",d);
            break;
         case TYPE_BOOLEAN:
            if (s[i].address)
            {
               printf("true");
            }
            else
            {
               printf("false");
            }
            break;
         case TYPE_STRING: // string
            adr = (char*) (s[i].address);
            CI_print_a_string(adr,false);
            break;
         case TYPE_PTR: // pointer
         {
            printf("%p",(void*)s[i].address);
            break;
         }
         case TYPE_OBJ: // object reference
            printf("objref %p",(void*)s[i].address);
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

void CI_print_a_string(char* ptr,bool b) {
   uint16_t len = ((*ptr) & 0xff) + (*(ptr + 1) << 8);
   CI_print_a_string2(ptr + 2, len);
   if (b)
   {
      printf("\n");
   }

}

void CI_print_a_string2(char* ptr, uint16_t len) {
   for (char* i = ptr; i < ptr + len; i++) {
      printf("%c",*i);
   }
}

/**
 *
 * call an external function
 *
 **/
void CI_call_external(short unsigned int function_number,short unsigned int a) {
   if (function_number > extern_count)
   {
        printf("illegal function number %d highest function number is %lu\n",function_number,extern_count);
        exit(-1);
   }
   struct extern_record e = externs[function_number-1];
   void* sym = (void*)(e.address);
   char* signature = e.signature;
   char* cpos = strchr(signature,'-');
   int pos;
   if (cpos == NULL) { pos=0; } else { pos = cpos - signature; } 
   bool varargs =  (pos==0);
   if (varargs)
   {
      cpos = strchr(signature,'+');
      if (cpos == NULL) { pos=0; } else { pos = cpos - signature; } 
   }
   char* ingoing = GC_MALLOC(pos+1);
   strncpy(ingoing, signature,pos);
   int ilen =pos;
   char* outgoing = GC_MALLOC(pos+1);
   strncpy(outgoing,signature,pos+1); 
   DCCallVM* vm = dcNewCallVM(4096);
   if (varargs)
   {
      if (a < ilen) 
      {
         printf("Mismatch: ingoing arguments given %d but expected at least %lu",a,pos);
         exit(-1);
      }
      dcMode(vm, DC_CALL_C_ELLIPSIS);
   }
   else
   {
      if (a != ilen) 
      {
         printf("Mismatch: ingoing arguments given %d but expected %lu",a,pos);
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
   for(int i=0;i<ilen;i++) {
      char* c = ingoing[i];
      struct stack_element f = s[t - cnt];
      CI_pass_in_arg(vm,c,f);
      cnt--;
   } 
   //
   //  in case of a vararg, there may be more args
   //
   int left = a - ilen;
   for (int i = 0; i< left;i++)
   {
      struct stack_element f = s[t - cnt];
      CI_pass_in_arg(vm,' ' ,f);
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
         char* ptr = (char*)GC_MALLOC(8);
         memcpy(ptr, &r, 8);
         tb--;
         s[t].atype = 5;
         s[t].address = (long long unsigned int) (ptr );
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


void CI_pass_in_arg( DCCallVM* vm, char c,struct stack_element f)
{
      uint16_t atype = f.atype;
      switch(atype)
      {
          case TYPE_INT: // int
          {
             if ((c != ' ' ) && (c!= 'i'))
             {
                printf("expected %c in external call but got %d\n",c,atype);
                exit(-1);
             }
             dcArgInt(vm,f.address);
             break;
          }
          case TYPE_FLOAT:
          {
             if ( (c != ' ' ) && ( c!= 'd'))
             {
                printf("expected double in external call but got %d\n",atype);
                exit(-1);
             }
             double arg_in;
             char* adr = (char*) (f.address);
             memcpy(&arg_in, adr, 8);
             dcArgDouble(vm, arg_in);
             break;
          }
          case TYPE_STRING: // char*
          {
             if ((c !=' ' ) && (c != 'p'))
             {
                printf("expected string or pointer in external call but got %d\n",atype);
                exit(-1);
             } 
                char* adr = (char*) (f.address);
                int len = ((*adr) & 0xff) + (*(adr + 1) << 8);
                char* str = (char*) GC_MALLOC(len + 1);
                memcpy(str, adr + 2, len);
                str[len] = '\0';
                dcArgPointer(vm, str);
                break;
            }
         case TYPE_PTR: // pointer
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
