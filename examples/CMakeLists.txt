# Include main evolution directory
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/src)

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

# Cairo
INCLUDE(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG(-lcairo HAVE_CAIRO)
IF (HAVE_CAIRO)
	MESSAGE("** Found Cairo")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcairo")
	INCLUDE_DIRECTORIES("/usr/include/cairo")
ELSE (HAVE_CAIRO)
	MESSAGE("!! Cannot link against Cairo")
ENDIF (HAVE_CAIRO)

# GNUplot library
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/lib/gnuplot)

# Compile library
ADD_LIBRARY(image image.h image.cpp)
TARGET_LINK_LIBRARIES(image ${Cairo_LIBRARIES})

# Build executale 1 (image_write)
ADD_EXECUTABLE(image_write image_write.cpp)

# Link executable 1 (image_write)
TARGET_LINK_LIBRARIES(image_write image)
TARGET_LINK_LIBRARIES(image_write population)
TARGET_LINK_LIBRARIES(image_write environment)
TARGET_LINK_LIBRARIES(image_write ${Cairo_LIBRARIES})

# Build executable 2 (image_benchmark)
ADD_EXECUTABLE(image_benchmark image_benchmark.cpp)

# Link executable 2 (image_bench)
TARGET_LINK_LIBRARIES(image_benchmark image)
TARGET_LINK_LIBRARIES(image_benchmark population)
TARGET_LINK_LIBRARIES(image_benchmark environment)
TARGET_LINK_LIBRARIES(image_benchmark ${Cairo_LIBRARIES})
TARGET_LINK_LIBRARIES(image_benchmark gnuplot)


#
# Tetris
#

# Add and include tetris library
ADD_SUBDIRECTORY("tetris")
INCLUDE_DIRECTORIES("tetris")

# Build executable
#ADD_EXECUTABLE(tetris tetris.h tetris.cpp)
#TARGET_LINK_LIBRARIES(tetris tetrislib)

