
# Include main evolution directory
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/src)


#
# Benchmark library
#

# Build library
ADD_LIBRARY(benchmark benchmark.h benchmark.cpp)


#
# DNA
#

# Build executable
ADD_EXECUTABLE(benchmark_dna benchmark_dna.cpp)

# Link executable
TARGET_LINK_LIBRARIES(benchmark_dna dna)
TARGET_LINK_LIBRARIES(benchmark_dna benchmark)

