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
TARGET_LINK_LIBRARIES(parser_grammar parser_type parser_value parser_function)

# Parser
ADD_LIBRARY(parser parser.h parser.cpp)
TARGET_LINK_LIBRARIES(parser parser_grammar)


#
# Build executable - simple
#

# Build executable
ADD_EXECUTABLE(simple simple.cpp)
TARGET_LINK_LIBRARIES(simple parser parser_function dna)

