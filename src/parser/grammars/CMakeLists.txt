#
# Build executable - simple
#

# Build executable
ADD_EXECUTABLE(simple_test simple_test.cpp)
TARGET_LINK_LIBRARIES(simple_test parser dna)

