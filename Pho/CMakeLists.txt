# CMakeList.txt : CMake project for Pho, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Pho "Main.cpp" "Main.h" "Tokenizer.cpp" "Tokenizer.h" "Parser.cpp" "Expression.h"  "Visitor.h" "Visitor.cpp" "Expression.cpp" "Environment.h" "Environment.cpp" "Interpreter.h" "Interpreter.cpp" "Error.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Pho PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
