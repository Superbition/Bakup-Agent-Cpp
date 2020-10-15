cmake_minimum_required(VERSION 3.7)
PROJECT(Bakup_Agent)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0079 NEW)

set(GTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/googletest/googletest")
set(gtest_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/googletest/googletest")

add_subdirectory(include/googletest EXCLUDE_FROM_ALL)
target_link_libraries(gtest_main PUBLIC gtest)
enable_testing()

set(USE_SYSTEM_CURL ON)
set(BUILD_CPR_TESTS OFF)
add_subdirectory(include/cpr)

set(RAPIDJSON_BUILD_TESTS OFF)
add_subdirectory(include/rapidjson)

add_library(inc include)
target_include_directories(inc PUBLIC include)
target_include_directories(inc PUBLIC ${CPR_INCLUDE_DIRS})
target_link_libraries(inc ${CPR_LIBRARIES})
link_directories(lib)

set(SOURCE_FILES main.cpp)
add_executable(Bakup_Agent main.cpp Debug.cpp Debug.h Agent.cpp Agent.h Request.cpp Request.h Command.cpp Command.h Response.cpp Response.h)
target_link_libraries(Bakup_Agent inc)