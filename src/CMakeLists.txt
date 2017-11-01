cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required

function(echo_all_cmake_variable_values)
  message(STATUS )
  get_cmake_property(vs VARIABLES)
  foreach(v ${vs})
    message(STATUS ${v}=${${v}}')
  endforeach(v)
  message(STATUS )
endfunction()

echo_all_cmake_variable_values()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
PROJECT(myparser)
ADD_EXECUTABLE(myparser Assignment.cpp CFunctions.c CInterpreter.c ClassDefinition.cpp CodeGenerator.cpp CommaSeparated.cpp DebugInfo.cpp Disassembler.cpp Expression.cpp ExpressionList.cpp Extern.cpp If.cpp LitBool.cpp Literal.cpp LitFloat.cpp LitInt.cpp LitNull.cpp LitString.cpp MethodCall.cpp PrintNode.cpp ProcedureCall.cpp ProcedureNode.cpp ProgramNode.cpp ReturnNode.cpp SingleMethodCall.cpp Statement.cpp Statements.cpp Val2Expression.cpp NotExpression.cpp VariableValue.cpp While.cpp Comments.cpp SelfTest.cpp Self.cpp Runner.cpp SelfStart.c FileCreator.cpp myparser.lex.c y.tab.c jwHash.c)
SET_PROPERTY(TARGET interpreter PROPERTY CMAKE_BUILD_TYPE C)
ADD_EXECUTABLE(interpret main.cpp CFunctions.c CInterpreter.c SelfStart.c jwHash.c)

INCLUDE_DIRECTORIES( ../dyncall-0.9/include ../dyncall-0.8_win/include ../bdwgc/include .)

IF(WIN32)
   LINK_DIRECTORIES(/usr/lib ../dyncall-0.8_win/lib ../dyncall-0.8_linux/lib /usr/lib/x86_64-linux-gnu ../bdwgc /c/MinGW/lib)
   ADD_LIBRARY(libdyncall_s STATIC IMPORTED)
   ADD_LIBRARY(libdynload_s STATIC IMPORTED)
   ADD_LIBRARY(gc.a STATIC IMPORTED)
   SET_PROPERTY(TARGET libdyncall_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdyncall_s.lib)
   SET_PROPERTY(TARGET libdynload_s PROPERTY IMPORTED_LOCATION ../dyncall-0.8_win/lib/libdynload_s.lib)
   SET_PROPERTY(TARGET gc.a PROPERTY IMPORTED_LOCATION ../bdwgc/gc.a)
   SET_TARGET_PROPERTIES(myparser PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
   TARGET_LINK_LIBRARIES(myparser libdyncall_s libdynload_s gc.a )
   TARGET_LINK_LIBRARIES(interpret libdyncall_s libdynload_s gc.a )
   SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-incompatible-pointer-types -Wno-int-conversion")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DDEBUG -Os -m32 -fno-exceptions -fno-rtti -fno-stack-protector -fomit-frame-pointer -ffunction-sections -fdata-sections -Wl,--gc-sections -mpreferred-stack-boundary=2 -falign-functions=1 -falign-jumps=1 -falign-loops=1 -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -Wl,--build-id=none -Wl,norelro -Wl-z -fshort-double -fsingle-precision-constant -mfpmath=387 -mfancy-math-387 -ffast-math --hash-style=gnu")
ENDIF(WIN32)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   LINK_DIRECTORIES(/usr/lib /usr/lib/x86_64-linux-gnu ../bdwgc ../dyncall-0.9/dyncall ../dyncall-0.9/dynload ../dyncall-0.9/dyncallback )
   ADD_LIBRARY(libgc-lib.a STATIC IMPORTED)
   #ADD_LIBRARY(libgcc_s.so.1 STATIC IMPORTED) 
   ADD_LIBRARY(libdyncall_s STATIC IMPORTED)
   ADD_LIBRARY(libdynload_s STATIC IMPORTED)
   ADD_LIBRARY(libgccpp STATIC IMPORTED)
   ADD_LIBRARY(dynload_s.a STATIC IMPORTED)
   ADD_LIBRARY(dyncall_s.a STATIC IMPORTED)
   ADD_LIBRARY(dyncallback_s.a STATIC IMPORTED)
   ADD_LIBRARY(libdl.so SHARED IMPORTED)
   SET_PROPERTY(TARGET libgc-lib.a PROPERTY IMPORTED_LOCATION ../bdwgc/libgc-lib.a)
#   SET_PROPERTY(TARGET libgcc_s.so.1 PROPERTY IMPORTED_LOCATION /lib/x86_64-linux-gnu/libgcc_s.so.1)
   SET_PROPERTY(TARGET dyncall_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.9/dyncall/libdyncall_s.a)
   SET_PROPERTY(TARGET dynload_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.9/dynload/libdynload_s.a)
   SET_PROPERTY(TARGET dyncallback_s.a PROPERTY IMPORTED_LOCATION ../dyncall-0.9/dyncallback/libdyncallback_s.a)
   SET_PROPERTY(TARGET libdl.so PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libdl.so)
   SET_TARGET_PROPERTIES(myparser PROPERTIES LINK_FLAGS "-Wl,--as-needed -fPIC")
   SET_TARGET_PROPERTIES(interpret PROPERTIES LINK_FLAGS "-Wl,--as-needed -fPIC")
   TARGET_LINK_LIBRARIES(myparser dyncall_s.a dynload_s.a dyncallback_s.a libdl.so pthread libgc-lib.a)
   TARGET_LINK_LIBRARIES(interpret dyncall_s.a dynload_s.a dyncallback_s.a libdl.so pthread libgc-lib.a)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Create target for the parser
ADD_CUSTOM_TARGET(YaccParser echo "Creating parser.c")
SET_PROPERTY(TARGET YaccParser PROPERTY COMPILE_FLAGS "-std=gnu++11 -SAFESEH:NO" )
SET_SOURCE_FILES_PROPERTIES( myparser.lex.c PROPERTIES COMPILE_FLAGS "-std=gnu++11" )

# for visual studio: SET (CMAKE_CXX_FLAGS "-fpermissive /EHsc")
# Create custom command for flex/lex (note the outputs)
ADD_CUSTOM_COMMAND(
   COMMAND flex 
   ARGS --outfile=myparser.lex.c myparser.lex
   TARGET YaccParser
   OUTPUTS myparser.lex.c)

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
   COMMAND bison
   ARGS  -d -v -t myparser.y --output y.tab.c
   TARGET YaccParser
   DEPENDS myparser.lex.c
   OUTPUTS y.tab.c)
#SET (CMAKE_CXX_FLAGS "-fpermissive")

IF(MSVC)
   SET (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO" )
   # for visual studio
   SET (CMAKE_CXX_FLAGS "-fpermissive /EHsc")
ENDIF(MSVC)

IF(NOT MSVC)
   # Create custom command for flex/lex (note the outputs)
   ADD_CUSTOM_COMMAND(
      COMMAND flex 
      ARGS --outfile=myparser.lex.c myparser.lex
      TARGET YaccParser
      OUTPUTS myparser.lex.c)
   
   # Create custom command for bison/yacc (note the DEPENDS)
   ADD_CUSTOM_COMMAND(
      COMMAND bison
      ARGS  -d -v -t myparser.y --output y.tab.c
      TARGET YaccParser
      DEPENDS myparser.lex.c
      OUTPUTS y.tab.c)
   SET (CMAKE_CXX_FLAGS "-fpermissive")
   #
   # add a cast to the result of the malloc() command
   # write the result to myparser.lex.c
   #
   FILE(READ lex.yy.c f0 )
     STRING( REGEX REPLACE "malloc[(]strlen" "(char*)GC_MALLOC(strlen" f2 "${f0}" )
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
ENDIF(NOT MSVC)
 
SET (CMAKE_BUILD_TYPE Release)

