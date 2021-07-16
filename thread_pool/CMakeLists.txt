# CMakeList.txt : CMake project for thread_pool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
# Add source to this project's executable.
add_executable (thread_pool "tset.cpp" "thread_pool.h" "worker.hpp" "event.hpp" "thread_utils.h" "thread_utils.cpp")

# TODO: Add tests and install targets if needed.
