cmake_minimum_required(VERSION 3.7)
PROJECT(Bakup_Agent)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_ROOT_DIR include/openssl)
set(OPENSSL_DIR include/openssl)
set(OPENSSL_INCLUDE_DIR include/openssl/apps/include)
set(OPENSSL_LIBRARY include/openssl/apps/lib)

include_directories(include)
link_directories(lib)

add_subdirectory(cpr)

set(SOURCE_FILES main.cpp)
add_executable(Bakup_Agent main.cpp)