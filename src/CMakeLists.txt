cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
PROJECT(myparser)
ADD_EXECUTABLE(myparser Assignment.cpp CFunctions.cpp CInterpreter.cpp ClassDefinition.cpp CodeGenerator.cpp CommaSeparated.cpp DebugInfo.cpp Disassembler.cpp Expression.cpp ExpressionList.cpp Extern.cpp If.cpp LitBool.cpp Literal.cpp LitFloat.cpp LitInt.cpp LitNull.cpp LitString.cpp MethodCall.cpp PrintNode.cpp ProcedureCall.cpp ProcedureNode.cpp ProgramNode.cpp ReturnNode.cpp SingleMethodCall.cpp Statement.cpp Statements.cpp Val2Expression.cpp NotExpression.cpp VariableValue.cpp While.cpp Comments.cpp SelfTest.cpp Self.cpp Runner.cpp myparser.lex.c y.tab.c)
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
INCLUDE_DIRECTORIES( ../dyncall-0.8_linux/include ../dyncall-0.8_win/include .)

IF(WIN32)
   LINK_DIRECTORIES(/usr/lib ../dyncall-0.8_win/lib ../dyncall-0.8_linux/lib /usr/lib/x86_64-linux-gnu)
ENDIF(WIN32)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   LINK_DIRECTORIES(/usr/lib /usr/lib/x86_64-linux-gnu ../dyncall-0.8_win/lib ../dyncall-0.8_linux/lib )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#ADD_LIBRARY(libgccpp STATIC IMPORTED)

IF(WIN32)
   ADD_LIBRARY(libdyncall_s STATIC IMPORTED)
   ADD_LIBRARY(libdynload_s STATIC IMPORTED)
   ADD_LIBRARY(libgc STATIC IMPORTED)
ENDIF(WIN32)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   ADD_LIBRARY(libdyncall_s STATIC IMPORTED)
   ADD_LIBRARY(libdynload_s STATIC IMPORTED)
   ADD_LIBRARY(libgc STATIC IMPORTED)
   ADD_LIBRARY(libgccpp STATIC IMPORTED)
   ADD_LIBRARY(dynload_s.a STATIC IMPORTED)
   ADD_LIBRARY(dyncall_s.a STATIC IMPORTED)
   ADD_LIBRARY(dyncallback_s.a STATIC IMPORTED)
   ADD_LIBRARY(libdl.so SHARED IMPORTED)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(WIN32)
   SET_PROPERTY(TARGET libdyncall_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdyncall_s.lib)
   SET_PROPERTY(TARGET libdynload_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdynload_s.lib)
ENDIF(WIN32)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET_PROPERTY(TARGET dyncall_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.8_linux/lib/libdyncall_s.a)
   SET_PROPERTY(TARGET dynload_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.8_linux/lib/libdynload_s.a)
   SET_PROPERTY(TARGET dyncallback_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.8_linux/lib/libdyncallback_s.a)
   SET_PROPERTY(TARGET libdl.so PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libdl.so)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")




SET(GCC_COVERAGE_COMPILE_FLAGS "-fpermissive")
ADD_DEFINITIONS(${GCC_COVERAGE_COMPILE_FLAGS})

IF(WIN32)
   TARGET_LINK_LIBRARIES(myparser libdyncall_s libdynload_s libgc.dll.a )
ENDIF(WIN32)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   TARGET_LINK_LIBRARIES(myparser dyncall_s.a dynload_s.a dyncallback_s.a libgc.a libgccpp.a libdl.so pthread)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Create target for the parser
ADD_CUSTOM_TARGET(YaccParser echo "Creating parser.c")
SET_PROPERTY(TARGET YaccParser PROPERTY COMPILE_FLAGS "-std=gnu++11 -SAFESEH:NO" )
SET_SOURCE_FILES_PROPERTIES( myparser.lex.c PROPERTIES COMPILE_FLAGS "-std=gnu++11" )

# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_COMMAND(
   COMMAND flex 
   ARGS myparser.lex
   TARGET YaccParser
   OUTPUTS myparser.lex.c)

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
   COMMAND bison
   ARGS -y -d -v -t myparser.y --output y.tab.c
   TARGET YaccParser
   DEPENDS myparser.lex.c
   OUTPUTS y.tab.c)
SET (CMAKE_CXX_FLAGS "-fpermissive")

# for visual studio: SET (CMAKE_CXX_FLAGS "-fpermissive /EHsc")

IF(MSVC)
   SET (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO" )
ENDIF(MSVC)

#
# add a cast to the result of the malloc() command
# write the result to myparser.lex.c
#
FILE(READ lex.yy.c f0 )
  STRING( REGEX REPLACE "malloc[(]strlen" "(char*)malloc(strlen" f2 "${f0}" )
FILE(WRITE myparser.lex.c "${f2}")

# Add y.tab.c to the list of sources
SET(myparser ${Foo_SRCS} ${Foo_BINARY_DIR}/src/y.tab.c)

# Since some files do not exist yet when cmake is run, mark
# them as generated
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/lex.yy.c GENERATED)
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/myparser.lex.c GENERATED)
SET_SOURCE_FILES_PROPERTIES(${Foo_BINARY_DIR}/src/y.tab.c GENERATED)

# Include binary directory to include lexer.c in parser.c
INCLUDE_DIRECTORIES(${Foo_BINARY_DIR}/src)

ADD_DEPENDENCIES(myparser YaccParser)

SET_SOURCE_FILES_PROPERTIES(lex.yy.c y.tab.c myparser.lex.c PROPERTIES LANGUAGE CXX )

