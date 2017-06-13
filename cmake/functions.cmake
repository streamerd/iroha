# Creates test "test_name", with "SOURCES" (use string as second argument)
function(AddTest test_name SOURCES)
    add_executable(${test_name} ${SOURCES})
    target_link_libraries(${test_name} PRIVATE gtest)
    add_test(
            NAME ${test_name}
            COMMAND $<TARGET_FILE:${test_name}>
    )
endfunction()

# COMPILE_PROTO function. Expects three variables:
# SRCS and HDRS. Will write path to compiled files in these variables.
# PROTO - path to proto file.
function(COMPILE_PROTO SRCS HDRS PROTO)
    if(PROTOBUF_FOUND)
        PROTOBUF_GENERATE_CPP(S H ${PROTO})
        set_source_files_properties(S H PROPERTIES GENERATED TRUE)
        set(${SRCS} ${S} PARENT_SCOPE)
        set(${HDRS} ${H} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "No protobuf found in COMPILE_PROTO function")
    endif()
endfunction()

# COMPILE_GRPC_PROTO function. Compiles GRPC stubs. Expects three variables:
# SRCS and HDRS. Will write path to compiled files in these variables.
# PROTO - path to proto file.
# to successfully compile GRPC stub you have to compile with both
# COMPILE_PROTO and COMPILE_GRPC_PROTO
function(COMPILE_GRPC_PROTO SRCS HDRS PROTO)
    if(PROTOBUF_FOUND)
        PROTOBUF_GENERATE_GRPC_CPP(S H ${PROTO})
        set_source_files_properties(S H PROPERTIES GENERATED TRUE)
        set(${SRCS} ${S} PARENT_SCOPE)
        set(${HDRS} ${H} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "No protobuf found in COMPILE_GRPC_PROTO function")
    endif()
endfunction()

