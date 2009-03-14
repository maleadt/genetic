# We require a sufficiently new version of CMake (for wxWidgets detection)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name
PROJECT(Evolve)

# Add an "profile" build type
SET(CMAKE_CXX_FLAGS_PROFILE "-pg -DNDEBUG" CACHE STRING
    "Flags used by the C++ compiler during profiled builds."
    FORCE)
SET(CMAKE_C_FLAGS_PROFILE "-pg -DNDEBUG" CACHE STRING
    "Flags used by the C compiler during profiled builds."
    FORCE)
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_PROFILE
    CMAKE_C_FLAGS_PROFILE)

# Type of build ("debug", "release" or "profile")
set(CMAKE_BUILD_TYPE profile)

# Add individual subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
