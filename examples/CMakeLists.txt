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
ELSE (HAVE_CAIRO)
	MESSAGE("!! Cannot link against Cairo")
ENDIF (HAVE_CAIRO)


# Build executale
ADD_EXECUTABLE(image image.cpp)

# Link executable
TARGET_LINK_LIBRARIES(image population)
TARGET_LINK_LIBRARIES(image environment)
TARGET_LINK_LIBRARIES(image ${Cairo_LIBRARIES})
