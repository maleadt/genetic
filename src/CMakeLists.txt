#
# Configuration
#

# Enable C++0x
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")


#
# Build libraries
#

# Generic functions
ADD_LIBRARY(generic generic.h generic.cpp)

# DNA datatype
ADD_LIBRARY(dna dna.h dna.cpp)

# Default environment
ADD_LIBRARY(environment environment.h environment.cpp)
TARGET_LINK_LIBRARIES(environment dna)

# Client
ADD_LIBRARY(client client.h client.cpp)
TARGET_LINK_LIBRARIES(client dna)
TARGET_LINK_LIBRARIES(client generic)

# Population
ADD_LIBRARY(population population.h population.cpp)
TARGET_LINK_LIBRARIES(population client)
TARGET_LINK_LIBRARIES(population environment)

