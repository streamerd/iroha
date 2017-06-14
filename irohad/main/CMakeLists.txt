set(SOURCE_FILES
        irohad.cc
        )

add_executable(iroha ${SOURCE_FILES})

target_link_libraries(iroha PRIVATE
        async_server
        )
