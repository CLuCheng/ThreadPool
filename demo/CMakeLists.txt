cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(demo)
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


add_executable(demo test.cpp)

target_link_libraries(demo PRIVATE thread_pool::core)
