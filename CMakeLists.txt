# We require a sufficiently new version of CMake (for wxWidgets detection)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name
PROJECT(Evolve)

# Add an "optimizedp4" build type
SET(CMAKE_CXX_FLAGS_OPTIMIZEDP4 "-march=pentium4 -O3 -pipe -fomit-frame-pointer" CACHE STRING
    "Flags used by the C++ compiler during optimized-for-p4 builds."
    FORCE)
SET(CMAKE_C_FLAGS_OPTIMIZEDP4 "-march=pentium4 -O3 -pipe -fomit-frame-pointer" CACHE STRING
    "Flags used by the C compiler during optimized-for-p4 builds."
    FORCE)
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_OPTIMIZEDP4
    CMAKE_C_FLAGS_OPTIMIZEDP4)

# Type of build ("debug", "release" or "optimizedp4")
set(CMAKE_BUILD_TYPE release)

# Add individual subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
