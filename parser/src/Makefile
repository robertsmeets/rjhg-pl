.PHONY: Debug
Debug: parser.exe

.PHONY: clean
clean:
	rm *.o

parser.o: parser.cpp
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o parser.o parser.cpp

ProcedureNode.o: ProcedureNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o ProcedureNode.o ProcedureNode.cpp

CFunctions.o: CFunctions.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o CFunctions.o CFunctions.cpp

ExpressionNode.o: ExpressionNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o ExpressionNode.o ExpressionNode.cpp

PrintNode.o: PrintNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o PrintNode.o PrintNode.cpp
   
IfNode.o: IfNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o IfNode.o IfNode.cpp
   
CodeGenerator.o: CodeGenerator.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  CodeGenerator.o CodeGenerator.cpp 
   
ExpressionParser.o: ExpressionParser.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  ExpressionParser.o ExpressionParser.cpp 
   
AssignmentNode.o: AssignmentNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  AssignmentNode.o AssignmentNode.cpp 
   
ProgramNode.o: ProgramNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  ProgramNode.o ProgramNode.cpp 
   
DebugInfo.o: DebugInfo.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  DebugInfo.o DebugInfo.cpp 
   
Statement.o: Statement.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  Statement.o Statement.cpp 
   
HeapManager.o: HeapManager.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  HeapManager.o HeapManager.cpp 
   
WhileNode.o: WhileNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  WhileNode.o WhileNode.cpp 
   
CInterpreter.o: CInterpreter.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  CInterpreter.o CInterpreter.cpp 
   
PException.o: PException.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  PException.o PException.cpp 
   
ProcedureCallNode.o: ProcedureCallNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  ProcedureCallNode.o ProcedureCallNode.cpp 
   
Disassembler.o: Disassembler.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  Disassembler.o Disassembler.cpp 
   
ExpressionThing.o: ExpressionThing.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  ExpressionThing.o ExpressionThing.cpp 
   
Parse.o: Parse.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  Parse.o Parse.cpp 
   
ReturnNode.o: ReturnNode.cpp 
	g++ -std=c++0x -I../../../dyncall-0.8_win/include -O0 -g3 -Wall -c -fmessage-length=0 -o  ReturnNode.o ReturnNode.cpp 

n22parser.exe: Statement.o ReturnNode.o ProgramNode.o ProcedureNode.o ProcedureCallNode.o PrintNode.o Parse.o PException.o IfNode.o HeapManager.o ExpressionThing.o ExpressionParser.o ExpressionNode.o Disassembler.o DebugInfo.o CodeGenerator.o CInterpreter.o CFunctions.o AssignmentNode.o WhileNode.o 
	g++ -L"/c/MinGW/lib/gcc/mingw32/4.8.1/debug" -L"../../../dyncall-0.8_win/lib" -Xlinker -static -o parser.exe Statement.o ReturnNode.o ProgramNode.o ProcedureNode.o ProcedureCallNode.o PrintNode.o Parse.o PException.o IfNode.o HeapManager.o ExpressionThing.o ExpressionParser.o ExpressionNode.o Disassembler.o DebugInfo.o CodeGenerator.o CInterpreter.o CFunctions.o AssignmentNode.o WhileNode.o -llibdyncall_s -llibdynload_s 

parser.exe: Statement.o ReturnNode.o ProgramNode.o ProcedureNode.o ProcedureCallNode.o PrintNode.o Parse.o PException.o IfNode.o HeapManager.o ExpressionThing.o ExpressionParser.o ExpressionNode.o Disassembler.o DebugInfo.o CodeGenerator.o CInterpreter.o CFunctions.o AssignmentNode.o WhileNode.o 
	g++ -L"/c/MinGW/lib/gcc/mingw32/4.8.1/debug" -L"../../../dyncall-0.8_win/lib" -Xlinker -static -o parser.exe Statement.o ReturnNode.o ProgramNode.o ProcedureNode.o ProcedureCallNode.o PrintNode.o Parse.o PException.o IfNode.o HeapManager.o ExpressionThing.o ExpressionParser.o ExpressionNode.o Disassembler.o DebugInfo.o CodeGenerator.o CInterpreter.o CFunctions.o AssignmentNode.o WhileNode.o 
	