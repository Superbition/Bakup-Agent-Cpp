cmake_minimum_required(VERSION 3.7)
PROJECT(Bakup_Agent)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0079 NEW)

set(GTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/googletest/googletest")
set(gtest_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/googletest/googletest")

set(USE_SYSTEM_CURL ON)
set(BUILD_CPR_TESTS OFF)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
FetchContent_MakeAvailable(cpr)

set(RAPIDJSON_BUILD_TESTS OFF)
add_subdirectory(include/rapidjson)

add_subdirectory(src)
target_link_libraries(${BINARY}_run PRIVATE cpr::cpr)
target_include_directories(${BINARY}_run PRIVATE _deps/cpr-src/include/cpr)

include_directories(src)
add_subdirectory(tests)
add_subdirectory(include/googletest)
target_link_libraries(gtest_main PUBLIC gtest)
enable_testing()