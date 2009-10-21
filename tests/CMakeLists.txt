# Add all tests
ADD_TEST(DNA check_dna)

# Include main evolution directory
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/src)

# Include the Check library
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/lib/check)


#
# DNA
#

# Build executable
ADD_EXECUTABLE(check_dna check_dna.cpp)

# Link executable
TARGET_LINK_LIBRARIES(check_dna dna)
TARGET_LINK_LIBRARIES(check_dna check)
TARGET_LINK_LIBRARIES(check_dna check_run)

