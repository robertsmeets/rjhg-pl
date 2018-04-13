/*
 * CodeGenerator.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "CodeGenerator.h"

using namespace std;

CodeGenerator::CodeGenerator() {
   codesize = 60000;
   codebuffer = (char*) GC_MALLOC(codesize);
   here = 0;
   pn = NULL;
   di = NULL;
   opr_mapping['/'] = 5; // DIV
   opr_mapping['%'] = 6; // MOD
   opr_mapping['I'] = 14;
   opr_mapping['!'] = 15; // NOT
   opr_mapping['&'] = 16; // AND
   opr_mapping['|'] = 17; // OR
}

CodeGenerator::~CodeGenerator() {
}

uint16_t CodeGenerator::getHere() {
   return here;
}

char* CodeGenerator::getCodeBuffer() {
   return codebuffer;
}

ProgramNode* CodeGenerator::getProgramNode()
{
   return pn;
}

void CodeGenerator::start(ProgramNode* a_pn, DebugInfo* a_di,bool debug) {
   pn=a_pn;
   //
   // emit a magic number
   //
   emitByte('R');
   emitByte('J');
   emitByte('H');
   emitByte('G');
   emitByte('P');
   emitByte('L');
   //
   // leave some space for the start address of the code
   //
   emit2Byte(0);
   //
   // count the number of methods
   //
   uint16_t amount_of_methods = 0;
   for (auto const &a_class : a_pn->getClasses()) {
      amount_of_methods += a_class->getMethods().size();
   }
   //
   // generate the class numbers
   //
   a_pn->assignClassNumbers();
   //
   // save the start address of the ext procedure table
   //
   uint16_t start_ext_proc_table = 8 * amount_of_methods + 10;
   //
   // emit the ext procedure table
   //
   int i = 1;
   uint16_t the_index = start_ext_proc_table;
   int nExterns = a_pn->getExterns().size();
   for (auto an_extern:a_pn->getExterns())
   {
      an_extern->setNumber(i);
      void* ptr = an_extern->address();
      unsigned long long a = (unsigned long long)ptr;  
      for (int j = 0;j< 8;j++)
      { 
         *((char*) codebuffer + the_index) = a & 255;
         a = a >> 8;
         the_index++;
      }
      //
      // now save the signature as as string
      //
      string s = an_extern->getEstring();
      const char * ss = s.c_str();
      char signature[50];
      strncpy(signature, ss, 49);
      int len = strlen(signature);
      memcpy((char*) codebuffer + the_index,signature,len+1);
      the_index += len + 1;
      i++;
   }
   uint16_t offset = the_index;
   //
   // save the start address of the bytecode
   //
   *((char*) codebuffer + 6) = offset & 255;
   *((char*) codebuffer + 7) = offset >> 8;
   here = offset;
   //
   // save the start address of the proc table
   // 
   *((char*) codebuffer + 8) = start_ext_proc_table & 255;
   *((char*) codebuffer + 9) = start_ext_proc_table >> 8;
   //
   //
   di = a_di;
   pn = a_pn;
   //
   // set up a call to main
   //
   // put a dummy value on the stack
   //
   emit(1, 2, 0, NULL);
   //
   // emit INT
   //
   emit(6, 0, 0, NULL);
   //
   // emit CAL
   //
   emit(5, 0, 0, NULL);
   callpoints[here - 2] = "main";
   //
   // emit RET
   //
   emitByte(19);
   emit2Byte(0);
   //
   // generate all the procedures
   //
   vector<ProcedureNode*> procs;
   procs = pn->getProcedures();
   //
   // generate the code for all the procedures
   //
   for (vector<ProcedureNode*>::iterator it = procs.begin(); it != procs.end(); ++it) {
      ProcedureNode* a_proc = *it;
      string pname = a_proc->getName();
      procaddresses[pname] = a_proc;
      a_proc->emit(this, here, NULL);
   }
   //
   // also the methods
   //
   for (auto const &a_class : a_pn->getClasses()) {
      for (auto const &a_method : a_class->getMethods()) {
         a_method->emit(this, here, a_class);
      }
   }
   //
   // fix the proc addresses
   //
   fix_proc_addresses();
   //
   // emit the method table
   //
   the_index = 10;
   for (auto const &a_class : a_pn->getClasses()) {
      for (auto const &a_method : a_class->getMethods()) {
         uint16_t cnum = a_class->getClassNum();
         uint16_t mnum = a_method->getMethodNumber();
         uint16_t address = a_method->getProcAddress();
         *((char*) codebuffer + the_index) = cnum & 255;
         the_index++;
         *((char*) codebuffer + the_index) = cnum >> 8;
         the_index++;
         *((char*) codebuffer + the_index) = mnum & 255;
         the_index++;
         *((char*) codebuffer + the_index) = mnum >> 8;
         the_index++;
         *((char*) codebuffer + the_index) = address & 255;
         the_index++;
         *((char*) codebuffer + the_index) = address >> 8;
         the_index++;
         *((char*) codebuffer + the_index) = a_method->getParameters()->size();
         the_index++;
         *((char*) codebuffer + the_index) = a_method->getLocalVariables()->size();
         the_index++;
      }
   }
}

//
// emit a f,l,a combination
//
void CodeGenerator::emit(char f, unsigned short int l, unsigned short int a,
      Expression* s) {
   emitByte(f);
   emit2Byte(l);
   emit2Byte(a);
}

void CodeGenerator::emitByte(char b) {
   *((char*) codebuffer + here) = b;
   here++;
}

void CodeGenerator::emit2Byte(uint16_t val) {
   emitByte(val & 255);
   emitByte(val >> 8);
}

//
// emit the code for an operation
//
void CodeGenerator::emitOperation(char avalue, Expression* s) {
   if (avalue=='+') {emit(OPCODE_PLS,0,0,s);return;}
   if (avalue=='-') {emit(OPCODE_MIN,0,0,s);return;}
   if (avalue=='*') {emit(OPCODE_MUL,0,0,s);return;}
   if (avalue=='/') {emit(OPCODE_DIV,0,0,s);return;}
   if (avalue=='%') {emit(OPCODE_MOD,0,0,s);return;}
   if (avalue=='=') {emit(OPCODE_EQ,0,0,s);return;}
   if (avalue=='<') {emit(OPCODE_LT,0,0,s);return;}
   if (avalue=='G') {emit(OPCODE_GE,0,0,s);return;}
   if (avalue=='>') {emit(OPCODE_GT,0,0,s);return;}
   if (avalue=='L') {emit(OPCODE_LE,0,0,s);return;}
   if (avalue=='N') {emit(OPCODE_NE,0,0,s);return;}
   if (avalue=='I') {emit(OPCODE_I,0,0,s);return;}
   if (avalue=='&') {emit(OPCODE_AND,0,0,s);return;}
   if (avalue=='|') {emit(OPCODE_OR,0,0,s);return;}
   if (avalue=='!') {emit(OPCODE_NOT,0,0,s);return;}
   printf("Unexpected Operation %c\n" , avalue);
}

/**
 * correct the procedure call addresses
 *
 */
void CodeGenerator::fix_proc_addresses() {
   //
   // loop over the callpoints
   //
   for (map<uint16_t, string>::iterator it = callpoints.begin();
         it != callpoints.end(); ++it) {
      uint16_t call_address = it->first;
      string proc_name = it->second;
      //
      // look up the proc name
      //
      ProcedureNode* pn = procaddresses[proc_name];
      if (pn == NULL) {
         //
         // external function
         //
         // fix the INT depth to create room for local variables
         //
         // size of the local variables (zero)
         //
         *((char*) codebuffer + call_address - 5) = 0;
         //
         // size of the parameters (assume 1)
         //
         *((char*) codebuffer + call_address - 7) = 1;

      } else {
         uint16_t proc_address = pn->getProcAddress();
         //
         // found the address of the proc
         //
         fix(call_address, proc_address);
         //
         // also fix the INT depth to create room for local variables
         //
         // size of the local variables
         //
         *((char*) codebuffer + call_address - 5) = pn->getLocalVariables()->size();
         //
         // size of the parameters,  add 1 for the self pointer
         *((char*) codebuffer + call_address - 7) = pn->getParameters()->size(); // + 1;
      }

   }
}

void CodeGenerator::fix(uint16_t call_address, uint16_t dest_address) {
   *((char*) codebuffer + call_address) = dest_address & 255;
   *((char*) codebuffer + call_address + 1) = dest_address >> 8;
}

//
// add a call address (starting point of a proc)
//
void CodeGenerator::addCallAddress(uint16_t address, string proc_name) {
   callpoints[address] = proc_name;
}

void CodeGenerator::addCallToProc(string name, Expression* s) {
   //
   //
   ClassDefinition* a_class = pn->getClass(name);
   if (a_class != NULL) {
      //
      // it's a class constructor
      //
      addCallToClassConstructor(a_class, s);
   } else {

      if (name.find('.') != string::npos)
      {
         //
         // it's a method call
         //
      }
      else
      {
         //
         // it's a procedure
         //
         addCallToProcedure(name, s);
      }
   }
}

void CodeGenerator::addCallToMethod(string method_name, ProcedureNode* procn, Expression* s) {
   //
   //
   emit(12, pn->getMethodNumber(method_name), procn->getParameters()->size(), s);
}

void CodeGenerator::addCallToClassConstructor(ClassDefinition* cd, Expression* s) {
   uint16_t ivs = cd->getInstanceVariables().size();
   uint16_t classnum = cd->getClassNum();
   emit(11, classnum, ivs, s);
}

void CodeGenerator::addCallToProcedure(string procedure_name, Expression* s) 
{
   //
   // add room for the local variables.
   // emit an INT
   // Since we don't know how many, leave 0 for the INT parameter
   // this will be corrected in the fix stage
   //
   emit(6, 0, 0, s);
   //
   // determine if procedure_name was defined
   // in the program code, if not it's a dynamic call
   //
   Expression* proc = procDefined(procedure_name);
   if (proc != NULL) {
      //
      // emit a "cal"
      // leave the call address 0, since this will be corrected in the fix stage
      //
      emit(5, 0, 0, s);
      addCallAddress(here - 2, procedure_name);
   } else {
      //
      // dealing with a dynamic call, to a library function
      // The string is saved.
      //
      uint16_t strlen = procedure_name.length();
      emit(10, 1, strlen, s);
      addCallAddress(here - 2, procedure_name);
      memcpy(codebuffer + here, procedure_name.c_str(), strlen);
      here += strlen;
   }
}

Expression* CodeGenerator::procDefined(string procedure_name) {
   vector<ProcedureNode*> procedures = pn->getProcedures();
   for (vector<ProcedureNode*>::iterator it = procedures.begin();
         it != procedures.end(); ++it) {
      ProcedureNode* a_proc = *it;
      string pname = a_proc->getName();
      if (pname == procedure_name) {
         return (Expression*) a_proc;
      }
   }
   return NULL;
}


char* CodeGenerator::allot(int size)
{
   uint16_t loc = here;
   here += size;
   return codebuffer + loc;
}

ClassDefinition* CodeGenerator::getClassDefinition(string name)
{
   return pn->getClass(name);
}
