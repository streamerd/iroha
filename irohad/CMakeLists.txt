include_directories(.)

# connection with CLIENT -> PEER
add_subdirectory(api)
# consensus algorithm
add_subdirectory(consensus)
# connection PEER -> STORAGE
add_subdirectory(storage)


set(SOURCE_FILES
        peer.cc
        )

add_executable(iroha ${SOURCE_FILES})

target_link_libraries(iroha PRIVATE
        async_server

        )

