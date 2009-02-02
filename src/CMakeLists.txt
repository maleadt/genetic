# Generic functions
ADD_LIBRARY(generic generic.h generic.cpp)

# DNA parser
ADD_LIBRARY(parser parser.h parser.cpp)

# Default environment
ADD_LIBRARY(environment environment.h environment.cpp)
TARGET_LINK_LIBRARIES(environment parser)

# Client
ADD_LIBRARY(client client.h client.cpp)
TARGET_LINK_LIBRARIES(client parser)
TARGET_LINK_LIBRARIES(client generic)

# Population
ADD_LIBRARY(population population.h population.cpp)
TARGET_LINK_LIBRARIES(population client)
TARGET_LINK_LIBRARIES(population environment)

