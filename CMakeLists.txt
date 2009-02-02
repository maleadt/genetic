# We require a sufficiently new version of CMake (for wxWidgets detection)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Project name
PROJECT(Evolve)

# Type of build ("debug" or "release")
set(CMAKE_BUILD_TYPE debug)

# Add individual subdirectories
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)

