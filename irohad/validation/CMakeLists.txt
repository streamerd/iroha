add_subdirectory(stateful)
add_subdirectory(chain)

add_library(stateless_validator
    stateless_validator.cpp
    )

target_link_libraries(stateless_validator
    schema
    optional
    ed25519
    rxcpp
    model
    )
