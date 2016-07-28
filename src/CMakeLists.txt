cmake_minimum_required(VERSION 3.2.2)
PROJECT(myparser)
ADD_EXECUTABLE(myparser Assignment.cpp CFunctions.cpp CInterpreter.cpp ClassDefinition.cpp CodeGenerator.cpp CommaSeparated.cpp DebugInfo.cpp Disassembler.cpp Expression.cpp ExpressionList.cpp Extern.cpp HeapManager.cpp If.cpp LitBool.cpp Literal.cpp LitFloat.cpp LitInt.cpp LitNull.cpp LitString.cpp MethodCall.cpp PrintNode.cpp ProcedureCall.cpp ProcedureNode.cpp ProgramNode.cpp ReturnNode.cpp SingleMethodCall.cpp Statement.cpp Statements.cpp Val2Expression.cpp VariableValue.cpp While.cpp myparser.lex.c y.tab.c)
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
ADD_DEFINITIONS(-std=c++11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
INCLUDE_DIRECTORIES( ../dyncall-0.8_linux/include ../dyncall-0.8_win/include)

LINK_DIRECTORIES(../dyncall-0.8_win/lib ../dyncall-0.8_linux/lib /usr/lib/x86_64-linux-gnu)

ADD_LIBRARY(libdyncall_s STATIC IMPORTED)
ADD_LIBRARY(libdynload_s STATIC IMPORTED)
ADD_LIBRARY(dynload_s.a STATIC IMPORTED)
ADD_LIBRARY(dyncall_s.a STATIC IMPORTED)
ADD_LIBRARY(dyncallback_s.a STATIC IMPORTED)
# library dl may be required on linux
#ADD_LIBRARY(dl STATIC IMPORTED)

SET_PROPERTY(TARGET libdyncall_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdyncall_s.lib)
SET_PROPERTY(TARGET libdynload_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdynload_s.lib)
SET_PROPERTY(TARGET dyncall_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.8_linux/lib/libdyncall_s.a)
SET_PROPERTY(TARGET dynload_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.8_linux/lib/libdynload_s.a)
SET_PROPERTY(TARGET dyncallback_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.8_linux/lib/libdyncallback_s.a)
#SET_PROPERTY(TARGET dl PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libdl.so)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wwrite-strings -fpermissive")
ADD_DEFINITIONS(${GCC_COVERAGE_COMPILE_FLAGS})
TARGET_LINK_LIBRARIES(myparser libdyncall_s libdynload_s dyncall_s.a dynload_s.a dyncallback_s.a)

# Create target for the parser
ADD_CUSTOM_TARGET(YaccParser echo "Creating parser.c")
SET_PROPERTY(TARGET YaccParser PROPERTY COMPILE_FLAGS "-std=gnu++11" )
SET_SOURCE_FILES_PROPERTIES( myparser.lex.c PROPERTIES COMPILE_FLAGS "-std=gnu++11" )

# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_COMMAND(
   COMMAND flex 
   ARGS myparser.lex 
   TARGET YaccParser
   OUTPUTS lex.yy.c)

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
   COMMAND byacc
   ARGS -d -v -t myparser.y
   TARGET YaccParser
   DEPENDS myparser.lex.c
   OUTPUTS y.tab.c)
SET (CMAKE_CXX_FLAGS "-fpermissive")

FILE(READ lex.yy.c f0 )
  STRING( REGEX REPLACE "malloc[(]strlen" "(char*)malloc(strlen" f2 "${f0}" )
FILE(WRITE myparser.lex.c "${f2}")

# Add parser.c to the list of sources
SET(myparser ${Foo_SRCS} ${Foo_BINARY_DIR}/src/y.tab.c)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/lex.yy.c GENERATED)
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/yy.tab.c GENERATED)

# Include binary directory to include lexer.c in parser.c
INCLUDE_DIRECTORIES(${Foo_BINARY_DIR}/src)

ADD_DEPENDENCIES(myparser YaccParser)

SET_SOURCE_FILES_PROPERTIES(lex.yy.c y.tab.c myparser.lex.c PROPERTIES LANGUAGE CXX )

