#
# Build libraries
#

# Generic functions
ADD_LIBRARY(generic generic.h generic.cpp)

# DNA datatype
ADD_LIBRARY(dna dna.h dna.cpp)

# Client
ADD_LIBRARY(client client.h client.cpp)
TARGET_LINK_LIBRARIES(client dna)
TARGET_LINK_LIBRARIES(client generic)

# Code parser
ADD_SUBDIRECTORY(parser)

# Environments
ADD_LIBRARY(environment environment.h environment.cpp)
TARGET_LINK_LIBRARIES(environment dna)
ADD_SUBDIRECTORY(environments)

# Populations
ADD_LIBRARY(population population.h population.cpp)
TARGET_LINK_LIBRARIES(population client)
TARGET_LINK_LIBRARIES(population environment)
ADD_SUBDIRECTORY(populations)

