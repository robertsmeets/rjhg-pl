.PHONY: clean
.PHONY: test

CFLAGS=-std=c++11 -g

myparser.exe: myparser.lex.o myparser.y.o Assignment.o Return.o While.o If.o CommaSeparated.o CompositeMethodCall.o Expression.o ExpressionList.o LitBool.o Literal.o LitFloat.o LitInt.o LitString.o pClassDefinition.o pMethodDefinition.o pProcedureNode.o pProgramNode.o ProcedureCall.o SingleMethodCall.o Statement.o Statements.o Val2Expression.o VariableValue.o
	g++ -g -o myparser.exe myparser.lex.o myparser.y.o Assignment.o Return.o While.o If.o CommaSeparated.o CompositeMethodCall.o Expression.o ExpressionList.o LitBool.o Literal.o LitFloat.o LitInt.o LitString.o pClassDefinition.o pMethodDefinition.o pProcedureNode.o pProgramNode.o ProcedureCall.o SingleMethodCall.o Statement.o Statements.o Val2Expression.o VariableValue.o

# add -v -t for debug
y.tab.h: myparser.y
	byacc -d myparser.y

y.tab.c: myparser.y
	byacc -d myparser.y
	
myparser.lex.o: myparser.lex.c y.tab.h
	g++ -g -c myparser.lex.c -o myparser.lex.o

myparser.y.o: y.tab.c
	g++ -Wno-write-strings $(CFLAGS) -c y.tab.c -o myparser.y.o

myparser.lex.c: myparser.lex
	flex myparser.lex
	cat lex.yy.c | sed s/malloc\(strlen/\(char*\)malloc\(strlen/g >myparser.lex.c

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

Statement.o: Statement.h Statement.cpp
	g++ $(CFLAGS) -o Statement.o -c Statement.cpp

Statements.o: Statements.h Statements.cpp
	g++ $(CFLAGS) -o Statements.o -c Statements.cpp

Val2Expression.o: Val2Expression.h Val2Expression.cpp
	g++ $(CFLAGS) -o Val2Expression.o -c Val2Expression.cpp

VariableValue.o: VariableValue.h VariableValue.cpp
	g++ $(CFLAGS) -o VariableValue.o -c VariableValue.cpp

clean:
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
	rm -f Statement.o
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

test: myparser.exe
	./myparser.exe < example.src	
