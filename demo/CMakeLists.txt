cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


add_executable(demo test.cpp)

add_dependencies(demo thread_pool)

target_link_libraries(demo thread_pool)
