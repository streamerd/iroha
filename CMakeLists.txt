cmake_minimum_required(VERSION 3.5)
project(iroha C CXX)

option(TESTING "Build tests" ON)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -fPIE")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -Wno-unused-parameter -O0")
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
SET(BINARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

include(cmake/dependencies.cmake)
include_directories(core)
add_subdirectory(core)

if(TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
