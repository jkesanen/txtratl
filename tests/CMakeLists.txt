cmake_minimum_required(VERSION 3.12)

project(txtratl_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    message(STATUS "Forcing gtest_force_shared_crt ${gtest_force_shared_crt} in ${CMAKE_CURRENT_LIST_FILE}")
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

list(APPEND TEST_SOURCES
    test_image.cpp
)

add_executable(txtratl_tests ${TEST_SOURCES})

target_link_libraries(txtratl_tests gtest gtest_main txtratl)

enable_testing()

add_test(NAME txtratl_tests COMMAND txtratl_tests)
