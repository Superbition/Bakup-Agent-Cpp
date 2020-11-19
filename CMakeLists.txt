cmake_minimum_required(VERSION 3.7)
PROJECT(Bakup_Agent)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0079 NEW)

include_directories(src)

set(USE_SYSTEM_CURL ON CACHE BOOL ON FORCE)
set(BUILD_CPR_TESTS OFF CACHE BOOL OFF FORCE)
add_subdirectory(include/cpr)

set(RAPIDJSON_BUILD_TESTS OFF)
add_subdirectory(include/rapidjson)

add_subdirectory(src)
target_link_libraries(${BINARY}_run PRIVATE cpr)
target_link_libraries(${BINARY}_lib PRIVATE cpr)

add_subdirectory(include/googletest)
add_subdirectory(tests)
target_link_libraries(${CMAKE_PROJECT_NAME}_tst PRIVATE cpr)
enable_testing()