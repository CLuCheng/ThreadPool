# CMakeList.txt : CMake project for thread_pool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(thread_pool)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


set(SRC_LIST  "include/thread_pool.h" "include/worker.hpp" "include/event.hpp" "include/thread_utils.h" "thread_utils.cpp")

# Add source to this project's executable.
if(USE_DYNAMIC)
    add_library(thread_pool SHARED ${SRC_LIST})
else()
    add_library(thread_pool STATIC ${SRC_LIST})
endif()

target_include_directories(
        thread_pool PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# TODO: Add tests and install targets if needed.
