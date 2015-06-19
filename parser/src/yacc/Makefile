.PHONY: clean
.PHONY: test

myparser.exe: myparser.lex.o myparser.y.o Assignment.o CommaSeparated.o CompositeMethodCall.o Expression.o ExpressionList.o LitBool.o Literal.o LitFloat.o LitInt.o LitString.o pClassDefinition.o pMethodDefinition.o pProcedureNode.o pProgramNode.o ProcedureCall.o SingleMethodCall.o Statement.o Statements.o Val2Expression.o
	g++ -g -o myparser.exe myparser.lex.o myparser.y.o Assignment.o CommaSeparated.o CompositeMethodCall.o Expression.o ExpressionList.o LitBool.o Literal.o LitFloat.o LitInt.o LitString.o pClassDefinition.o pMethodDefinition.o pProcedureNode.o pProgramNode.o ProcedureCall.o SingleMethodCall.o Statement.o Statements.o Val2Expression.o

y.tab.h: myparser.y
	byacc -d -v myparser.y

y.tab.c: myparser.y
	byacc -d -v myparser.y
	
myparser.lex.o: myparser.lex.c y.tab.h
	g++ -g -c myparser.lex.c -o myparser.lex.o

myparser.y.o: y.tab.c
	g++ -Wno-write-strings -std=c++11 -g -c y.tab.c -o myparser.y.o

myparser.lex.c: myparser.lex
	flex myparser.lex
	cat lex.yy.c | sed s/malloc\(strlen/\(char*\)malloc\(strlen/g >myparser.lex.c

Assignment.o: Assignment.h Assignment.cpp
	g++ -g -o Assignment.o -c Assignment.cpp

CommaSeparated.o: CommaSeparated.h CommaSeparated.cpp
	g++ -g -o CommaSeparated.o -c CommaSeparated.cpp

CompositeMethodCall.o: CompositeMethodCall.h CompositeMethodCall.cpp
	g++ -g -o CompositeMethodCall.o -c CompositeMethodCall.cpp

Expression.o: Expression.h Expression.cpp
	g++ -g -o Expression.o -c Expression.cpp

ExpressionList.o: ExpressionList.h ExpressionList.cpp
	g++ -g -o ExpressionList.o -c ExpressionList.cpp

LitBool.o: LitBool.h LitBool.cpp
	g++ -g -o LitBool.o -c LitBool.cpp

Literal.o: Literal.h Literal.cpp
	g++ -g -o Literal.o -c Literal.cpp

LitFloat.o: LitFloat.h LitFloat.cpp
	g++ -g -o LitFloat.o -c LitFloat.cpp

LitInt.o: LitInt.h LitInt.cpp
	g++ -g -o LitInt.o -c LitInt.cpp

LitString.o: LitString.h LitString.cpp
	g++ -g -o LitString.o -c LitString.cpp

pClassDefinition.o: pClassDefinition.h pClassDefinition.cpp
	g++ -g -o pClassDefinition.o -c pClassDefinition.cpp

pMethodDefinition.o: pMethodDefinition.h pMethodDefinition.cpp
	g++ -g -o pMethodDefinition.o -c pMethodDefinition.cpp

pProcedureNode.o: pProcedureNode.h pProcedureNode.cpp
	g++ -g -o pProcedureNode.o -c pProcedureNode.cpp

pProgramNode.o: pProgramNode.h pProgramNode.cpp
	g++ -g -o pProgramNode.o -c pProgramNode.cpp

ProcedureCall.o: ProcedureCall.h ProcedureCall.cpp
	g++ -g -o ProcedureCall.o -c ProcedureCall.cpp

SingleMethodCall.o: SingleMethodCall.h SingleMethodCall.cpp
	g++ -g -o SingleMethodCall.o -c SingleMethodCall.cpp

Statement.o: Statement.h Statement.cpp
	g++ -g -o Statement.o -c Statement.cpp

Statements.o: Statements.h Statements.cpp
	g++ -g -o Statements.o -c Statements.cpp

Val2Expression.o: Val2Expression.h Val2Expression.cpp
	g++ -g -o Val2Expression.o -c Val2Expression.cpp

clean:
	rm -f Assignment.o
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
