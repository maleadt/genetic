# Include main evolution directory
INCLUDE_DIRECTORIES(${EVOLVE_SOURCE_DIR}/src)

#
# Debug
#

# Build executable
ADD_EXECUTABLE(debug debug.cpp)

# Link executable
TARGET_LINK_LIBRARIES(debug parser)
TARGET_LINK_LIBRARIES(debug population)
TARGET_LINK_LIBRARIES(debug environment)


#
# Image
#

# Include wxWidgets
SET(wxWidgets_USE_LIBS base core) 
FIND_PACKAGE(wxWidgets REQUIRED) 
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
ADD_DEFINITIONS(${wxWidgets_DEFINITIONS})

# Build executale
ADD_EXECUTABLE(image image.cpp)

# Link executable
TARGET_LINK_LIBRARIES(image parser)
TARGET_LINK_LIBRARIES(image population)
TARGET_LINK_LIBRARIES(image environment)
TARGET_LINK_LIBRARIES(image ${wxWidgets_LIBRARIES})
