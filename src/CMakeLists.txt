# DNA parser
ADD_LIBRARY(parser parser.h parser.cpp)

# Client
ADD_LIBRARY(client client.h client.cpp)
TARGET_LINK_LIBRARIES(client parser)

# Default environment
ADD_LIBRARY(environment environment.h environment.cpp)
TARGET_LINK_LIBRARIES(environment parser)

