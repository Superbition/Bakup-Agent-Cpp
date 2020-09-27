cmake_minimum_required(VERSION 3.7)
PROJECT(Bakup_Agent)

set(CMAKE_CXX_STANDARD 17)


set(USE_SYSTEM_CURL ON)
add_subdirectory(include/cpr)
add_subdirectory(include/rapidjson)
add_library(inc include)
target_include_directories(inc PUBLIC include)
target_include_directories(inc PUBLIC ${CPR_INCLUDE_DIRS})
target_link_libraries(inc ${CPR_LIBRARIES})
link_directories(lib)

set(SOURCE_FILES main.cpp)
add_executable(Bakup_Agent main.cpp Debug.cpp Debug.h Agent.cpp Agent.h Request.cpp Request.h Command.cpp Command.h Response.cpp Response.h)
target_link_libraries(Bakup_Agent inc)