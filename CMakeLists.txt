# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("thread_pool" VERSION 1.0.0 LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

option( ENABLE_TEST "enable test for thread_pool" ON)
option( USE_DYNAMIC "compiler dynamic library" ON)

# Include sub-projects.
add_subdirectory ("thread_pool")

if (ENABLE_TEST)
    enable_testing()
    add_subdirectory("test")
endif()
