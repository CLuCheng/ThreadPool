# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("thread_pool" VERSION 1.0.0 LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

option( USE_DEMO "compiler demo" ON)
option( USE_DYNAMIC "compiler dynamic library" ON)

# Include sub-projects.
add_subdirectory ("thread_pool")

if (USE_DEMO)
    add_subdirectory("demo")
endif()
