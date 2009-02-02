# Generic functions
ADD_LIBRARY(generic generic.h generic.cpp)

# DNA parser
ADD_LIBRARY(parser parser.h parser.cpp)

# Client
ADD_LIBRARY(client client.h client.cpp)
TARGET_LINK_LIBRARIES(client parser)
TARGET_LINK_LIBRARIES(client generic)

# Default environment
ADD_LIBRARY(environment environment.h environment.cpp)
TARGET_LINK_LIBRARIES(environment parser)

