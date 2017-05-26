project(peer C)

set(SOURCE_FILES
        peer.c
        )

add_executable(iroha ${SOURCE_FILES})

target_link_libraries(iroha PRIVATE
        uv
        )


add_subdirectory(storage)