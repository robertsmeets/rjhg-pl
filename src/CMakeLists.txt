cmake_minimum_required(VERSION 3.3.2)
project (myparser)
add_executable(myparser Assignment.cpp CFunctions.cpp CInterpreter.cpp ClassDefinition.cpp CodeGenerator.cpp CommaSeparated.cpp DebugInfo.cpp Disassembler.cpp Expression.cpp ExpressionList.cpp Extern.cpp HeapManager.cpp If.cpp LitBool.cpp Literal.cpp LitFloat.cpp LitInt.cpp LitNull.cpp LitString.cpp MethodCall.cpp PrintNode.cpp ProcedureCall.cpp ProcedureNode.cpp ProgramNode.cpp ReturnNode.cpp SingleMethodCall.cpp Statement.cpp Statements.cpp Val2Expression.cpp VariableValue.cpp While.cpp myparser.lex.c y.tab.c)
set_property(TARGET myparser PROPERTY CXX_STANDARD 11)
include_directories(../dyncall-0.8_win/include)
link_directories(../dyncall-0.8_win/lib)
add_library(libdyncall_s STATIC IMPORTED)
add_library(libdynload_s STATIC IMPORTED)
set_property(TARGET libdyncall_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdyncall_s.lib)
set_property(TARGET libdynload_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdynload_s.lib)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wwrite-strings -fpermissive")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
target_link_libraries(myparser libdyncall_s libdynload_s)

# Create target for the parser
ADD_CUSTOM_TARGET(FooParser echo "Creating parser.c")

# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_COMMAND(
   COMMAND flex 
   ARGS myparser.lex 
   TARGET FooParser
   OUTPUTS lex.yy.c)

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
   COMMAND byacc
   ARGS -d -v -t myparser.y
   TARGET FooParser
   DEPENDS myparser.lex.c
   OUTPUTS y.tab.c)
set (CMAKE_CXX_FLAGS "-fpermissive")

FILE(READ lex.yy.c f0 )
  STRING( REGEX REPLACE "malloc[(]strlen" "(char*)malloc(strlen" f2 "${f0}" )
FILE(WRITE myparser.lex.c "${f2}")


#ADD_CUSTOM_COMMAND(
#   COMMAND cat lex.yy.c | sed s/malloc\(strlen/\(char*\)malloc\(strlen/g >myparser.lex.c

# Add parser.c to the list of sources
SET(myparser ${Foo_SRCS} ${Foo_BINARY_DIR}/src/y.tab.c)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/lex.yy.c GENERATED)
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/yy.tab.c GENERATED)

# Include binary directory to include lexer.c in parser.c
INCLUDE_DIRECTORIES(${Foo_BINARY_DIR}/src)

ADD_DEPENDENCIES(myparser FooParser)

SET_SOURCE_FILES_PROPERTIES(lex.yy.c y.tab.c myparser.lex.c PROPERTIES LANGUAGE CXX )

