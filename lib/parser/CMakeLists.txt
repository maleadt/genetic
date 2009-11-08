# Require minimum CMake version
cmake_minimum_required(VERSION 2.6)

# Include main evolution directory
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/src)

# Enable warnings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Enable C++0x
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# Type of build ("debug", "release" or "profile")
set(CMAKE_BUILD_TYPE debug)

#
# Build libraries
#

# Type
ADD_LIBRARY(parser_type type.h type.cpp)

# Value
ADD_LIBRARY(parser_value value.h value.cpp)
TARGET_LINK_LIBRARIES(parser_value parser_type)

# Function
ADD_LIBRARY(parser_function function.h function.cpp)
TARGET_LINK_LIBRARIES(parser_function parser_value parser_type)

# Grammar
ADD_LIBRARY(parser_grammar grammar.h grammar.cpp)
TARGET_LINK_LIBRARIES(parser_grammar parser_type parser_value)

# Parser
ADD_LIBRARY(parser parser.h parser.cpp)
TARGET_LINK_LIBRARIES(parser parser_grammar)


#
# Build executable - simple
#

# Build executable
ADD_EXECUTABLE(simple simple.cpp)
TARGET_LINK_LIBRARIES(simple parser parser_function)
