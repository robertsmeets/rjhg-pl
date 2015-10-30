.PHONY: clean
.PHONY: test

CFLAGS=-std=c++11 -g

myparser.exe: myparser.lex.o myparser.y.o Assignment.o Return.o While.o If.o CommaSeparated.o CompositeMethodCall.o Expression.o ExpressionList.o LitBool.o Literal.o LitFloat.o LitInt.o LitString.o pClassDefinition.o pMethodDefinition.o pProcedureNode.o pProgramNode.o ProcedureCall.o SingleMethodCall.o Statements.o Val2Expression.o VariableValue.o CodeGenerator.o PException.o

	g++ -g -o myparser.exe myparser.lex.o myparser.y.o Assignment.o Return.o While.o If.o CommaSeparated.o CompositeMethodCall.o Expression.o ExpressionList.o LitBool.o Literal.o LitFloat.o LitInt.o LitString.o pClassDefinition.o pMethodDefinition.o pProcedureNode.o pProgramNode.o ProcedureCall.o SingleMethodCall.o Statements.o Val2Expression.o VariableValue.o CodeGenerator.o PException.o

# add -v -t for debug

y.tab.c y.tab.h: myparser.y
	byacc -d -v -t myparser.y

# 
# Some functions go beyond the ANSI standard. These are disabled when you use std=c++11 (or std=c++0x).
# 
# Among them are fdopen, fileno and strdup. There are two possibilities to use them:
# 
#     Use the GNU dialect (std=gnu++11).
#    If you want to compile without dialect and make a local exception, you can include stdio.h with the __STRICT_ANSI__ undefined. (see: Error "'fdopen' was not declared" found with g++ 4 that compiled with g++3)
#
	
myparser.lex.o: myparser.lex.c y.tab.h
	g++ -std=gnu++11 -g -o myparser.lex.o -c myparser.lex.c

myparser.y.o: y.tab.c
	g++ -Wno-write-strings $(CFLAGS) -c y.tab.c -o myparser.y.o

myparser.lex.c: myparser.lex
	flex myparser.lex
	cat lex.yy.c | sed s/malloc\(strlen/\(char*\)malloc\(strlen/g >myparser.lex.c

PException.o: PException.h PException.cpp
	g++ $(CFLAGS) -o PException.o -c PException.cpp

Assignment.o: Assignment.h Assignment.cpp
	g++ $(CFLAGS) -o Assignment.o -c Assignment.cpp

Return.o: Return.h Return.cpp
	g++ $(CFLAGS) -o Return.o -c Return.cpp

While.o: While.h While.cpp
	g++ $(CFLAGS) -o While.o -c While.cpp

If.o: If.h If.cpp
	g++ $(CFLAGS) -o If.o -c If.cpp

CommaSeparated.o: CommaSeparated.h CommaSeparated.cpp
	g++ $(CFLAGS) -o CommaSeparated.o -c CommaSeparated.cpp

CompositeMethodCall.o: CompositeMethodCall.h CompositeMethodCall.cpp
	g++ $(CFLAGS) -o CompositeMethodCall.o -c CompositeMethodCall.cpp

Expression.o: Expression.h Expression.cpp
	g++ $(CFLAGS) -o Expression.o -c Expression.cpp

ExpressionList.o: ExpressionList.h ExpressionList.cpp
	g++ $(CFLAGS) -o ExpressionList.o -c ExpressionList.cpp

LitBool.o: LitBool.h LitBool.cpp
	g++ $(CFLAGS) -o LitBool.o -c LitBool.cpp

Literal.o: Literal.h Literal.cpp
	g++ $(CFLAGS) -o Literal.o -c Literal.cpp

LitFloat.o: LitFloat.h LitFloat.cpp
	g++ $(CFLAGS) -o LitFloat.o -c LitFloat.cpp

LitInt.o: LitInt.h LitInt.cpp
	g++ $(CFLAGS) -o LitInt.o -c LitInt.cpp

LitString.o: LitString.h LitString.cpp
	g++ $(CFLAGS) -o LitString.o -c LitString.cpp

pClassDefinition.o: pClassDefinition.h pClassDefinition.cpp
	g++ $(CFLAGS) -o pClassDefinition.o -c pClassDefinition.cpp

pMethodDefinition.o: pMethodDefinition.h pMethodDefinition.cpp
	g++ $(CFLAGS) -o pMethodDefinition.o -c pMethodDefinition.cpp

pProcedureNode.o: pProcedureNode.h pProcedureNode.cpp
	g++ $(CFLAGS) -o pProcedureNode.o -c pProcedureNode.cpp

pProgramNode.o: pProgramNode.h pProgramNode.cpp
	g++ $(CFLAGS) -o pProgramNode.o -c pProgramNode.cpp

ProcedureCall.o: ProcedureCall.h ProcedureCall.cpp
	g++ $(CFLAGS) -o ProcedureCall.o -c ProcedureCall.cpp

SingleMethodCall.o: SingleMethodCall.h SingleMethodCall.cpp
	g++ $(CFLAGS) -o SingleMethodCall.o -c SingleMethodCall.cpp

Statements.o: Statements.h Statements.cpp
	g++ $(CFLAGS) -o Statements.o -c Statements.cpp

Val2Expression.o: Val2Expression.h Val2Expression.cpp
	g++ $(CFLAGS) -o Val2Expression.o -c Val2Expression.cpp

VariableValue.o: VariableValue.h VariableValue.cpp
	g++ $(CFLAGS) -o VariableValue.o -c VariableValue.cpp

CodeGenerator.o: CodeGenerator.h CodeGenerator.cpp
	g++ $(CFLAGS) -o CodeGenerator.o -c CodeGenerator.cpp

clean:
	rm -f CodeGenerator.o
	rm -f Assignment.o
	rm -f Return.o
	rm -f While.o
	rm -f If.o
	rm -f CommaSeparated.o
	rm -f CompositeMethodCall.o
	rm -f Expression.o
	rm -f ExpressionList.o
	rm -f LitBool.o
	rm -f Literal.o
	rm -f LitFloat.o
	rm -f LitInt.o
	rm -f LitString.o
	rm -f pClassDefinition.o
	rm -f pMethodDefinition.o
	rm -f pProcedureNode.o
	rm -f pProgramNode.o
	rm -f ProcedureCall.o
	rm -f SingleMethodCall.o
	rm -f Statements.o
	rm -f Val2Expression.o
	rm -f VariableValue.o
	rm -f myparser.lex.o
	rm -f myparser.y.o
	rm -f yywrap.o
	rm -f y.tab.h
	rm -f y.tab.c
	rm -f myparser.tab.h
	rm -f myparser.tab.c
	rm -f myparser.h
	rm -f myparser.y.c
	rm -f myparser.exe
	rm -f myparser.lex.c
	rm -f myparser.c
	rm -f y.output
	rm -f lex.yy.c
	rm -f AssignmentNode.o
	rm -f CFunctions.o
	rm -f CInterpreter.o
	rm -f ClassDefinition.o
	rm -f DebugInfo.o
	rm -f Disassembler.o
	rm -f ExpressionNode.o
	rm -f ExpressionParser.o
	rm -f ExpressionThing.o
	rm -f HeapManager.o
	rm -f IfNode.o
	rm -f Parse.o
	rm -f parser.o
	rm -f PException.o
	rm -f PrintNode.o
	rm -f ProcedureCallNode.o
	rm -f ProcedureNode.o
	rm -f ProgramNode.o
	rm -f ReturnNode.o
	rm -f Statement.o
	rm -f WhileNode.o

test: myparser.exe
	./myparser.exe example.src
	
all: myparser.exe

