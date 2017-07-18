add_library(stateful_validator
    impl/validator.cpp
    impl/stub_validator.cpp
    impl/stub_command_validator.cpp)

target_link_libraries(stateful_validator
    schema
    rxcpp
    optional
    )