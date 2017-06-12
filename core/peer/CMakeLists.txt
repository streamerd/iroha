cmake_minimum_required(VERSION 3.5)
project(peer CXX)

set(SOURCE_FILES
        peer.cc
        )

add_executable(iroha ${SOURCE_FILES})

target_link_libraries(iroha PRIVATE
        grpcServerRunner
        )


add_subdirectory(api)
