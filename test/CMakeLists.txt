cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(demo)

set (TESTNAME "thread_pool_test")

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

################################
# GTest
################################
project(googletest-git NONE)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.8.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

################################
# Tests
################################
# Add test cpp file
add_executable(${TESTNAME} test.cpp)


# Link test executable against gtest & gtest_main
target_link_libraries(${TESTNAME} gtest gtest_main)


add_dependencies(${TESTNAME} thread_pool::core)
target_link_libraries(${TESTNAME} thread_pool::core)

enable_testing()
add_test(${TESTNAME} ${TESTNAME})

