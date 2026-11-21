/**
 * constants and definitions for AST
 *
 */
#define AST_ASSIGNMENT 1
#define AST_CLASSDEFINITION 2
#define AST_COMMASEPARATED 3
#define AST_EXPRESSIONLIST 4
#define AST_EXTERN 5
#define AST_IF 6
#define AST_LITBOOL 7
#define AST_LITFLOAT 8
#define AST_LITINT 9
#define AST_LITNULL 10
#define AST_LITSTRING 11
#define AST_METHODCALL 12
#define AST_NOTEXPRESSION 13
#define AST_PRINTNODE 14
#define AST_PROCEDURECALL 15
#define AST_PROCEDURENODE 16
#define AST_RETURNNODE 17
#define AST_SELF 18
#define AST_STATEMENTS 19
#define AST_VAL2EXPRESSION 20
#define AST_VARIABLEVALUE 21
#define AST_WHILE 22

#include <stdbool.h>

union ASTdata {
        int intValue;
	bool boolValue;
	float floatValue;
	void* ptrValue;
    };


struct AST {
    char operand;
    union ASTdata data;
    void* two;
    void* three;
    void* four;
};
