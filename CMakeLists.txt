cmake_minimum_required(VERSION 3.7)
PROJECT(Bakup_Agent)
set(BINARY ${CMAKE_PROJECT_NAME})

# Set DEBUG to false if it is not passed as a parameter
set(DEBUG FALSE CACHE BOOL "")
message("DEBUG MODE: " ${DEBUG})

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0079 NEW)

include_directories(src)

set(USE_SYSTEM_CURL ON CACHE BOOL OFF FORCE)
set(BUILD_CPR_TESTS OFF CACHE BOOL OFF FORCE)
add_subdirectory(include/cpr)

set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL OFF FORCE)
add_subdirectory(include/rapidjson)
include_directories(include/rapidjson/include)

# If DEBUG is true, include the googletest subdirectory
# Must come before related test files as they are dependent on this
if(DEBUG)
    add_subdirectory(include/googletest)
endif()

add_subdirectory(src)
target_link_libraries(${BINARY} PRIVATE cpr)
target_link_libraries(${BINARY}_lib PRIVATE cpr)

if(DEBUG)
    target_link_libraries(${BINARY}_lib PUBLIC gtest)

    add_subdirectory(tests)
    target_link_libraries(${BINARY}_tst PRIVATE cpr)
    enable_testing()
endif()
