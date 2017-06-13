cmake_minimum_required(VERSION 3.5)
project(iroha C CXX)

option(TESTING "Build tests" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -fPIE")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -Wno-unused-parameter -O0")
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
SET(BINARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

SET(PROTO_DIR ${PROJECT_SOURCE_DIR}/proto)
message(STATUS ".proto files are located at ${PROTO_DIR}")

include(cmake/dependencies.cmake)
include(cmake/functions.cmake)
# reusable components are located at libs
include_directories(libs)

# components
add_subdirectory(libs)
add_subdirectory(peer)
add_subdirectory(examples)

if(TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
