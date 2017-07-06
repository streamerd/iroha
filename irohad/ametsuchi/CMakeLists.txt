add_library(ametsuchi
    impl/flat_file/flat_file.cpp

    impl/storage_impl.cpp
    impl/temporary_wsv_impl.cpp
    impl/mutable_storage_impl.cpp

    impl/postgres_wsv_query.cpp
    impl/postgres_command_executor.cpp

    storage_stub.cpp
    command_executor_stub.cpp
    mutable_storage_stub.cpp
    temporary_wsv_stub.cpp
    )

target_link_libraries(ametsuchi
    rxcpp
    optional
    pqxx
    cpp_redis
    )