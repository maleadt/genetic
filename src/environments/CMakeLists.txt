#
# Debug
#

# Build executable
ADD_EXECUTABLE(debug debug.cpp)

# Link executable
TARGET_LINK_LIBRARIES(debug dna)
TARGET_LINK_LIBRARIES(debug population)
TARGET_LINK_LIBRARIES(debug environment)


#
# Image
#

ADD_SUBDIRECTORY(image)


#
# Tetris
#

# Add and include tetris library
ADD_SUBDIRECTORY("tetris")

