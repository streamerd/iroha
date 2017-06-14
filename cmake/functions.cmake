# Creates test "test_name", with "SOURCES" (use string as second argument)
function(AddTest test_name SOURCES)
    add_executable(${test_name} ${SOURCES})
    target_link_libraries(${test_name} PRIVATE gtest gtest_main)
    add_test(
            NAME ${test_name}
            COMMAND $<TARGET_FILE:${test_name}>
    )
endfunction()

## COMPILE_PROTO function. Expects three variables:
## SRCS and HDRS. Will write path to compiled files in these variables.
## PROTO - path to proto file.
#function(COMPILE_PROTO SRCS HDRS PROTO)
#    if(PROTOBUF_FOUND)
#        PROTOBUF_GENERATE_CPP(S H ${PROTO})
#        set_source_files_properties(S H PROPERTIES GENERATED TRUE)
#        set(${SRCS} ${S} PARENT_SCOPE)
#        set(${HDRS} ${H} PARENT_SCOPE)
#    else()
#        message(FATAL_ERROR "No protobuf found in COMPILE_PROTO function")
#    endif()
#endfunction()
#
## COMPILE_GRPC_PROTO function. Compiles GRPC stubs. Expects three variables:
## SRCS and HDRS. Will write path to compiled files in these variables.
## PROTO - path to proto file.
## to successfully compile GRPC stub you have to compile with both
## COMPILE_PROTO and COMPILE_GRPC_PROTO
#function(COMPILE_GRPC_PROTO SRCS HDRS PROTO)
#    if(PROTOBUF_FOUND)
#        PROTOBUF_GENERATE_GRPC_CPP(S H ${PROTO})
#        set_source_files_properties(S H PROPERTIES GENERATED TRUE)
#        set(${SRCS} ${S} PARENT_SCOPE)
#        set(${HDRS} ${H} PARENT_SCOPE)
#    else()
#        message(FATAL_ERROR "No protobuf found in COMPILE_GRPC_PROTO function")
#    endif()
#endfunction()



function(compile_proto_to_cpp PROTO)
    string(REGEX REPLACE "\\.proto$" ".pb.h" GEN_PB_HEADER ${PROTO})
    string(REGEX REPLACE "\\.proto$" ".pb.cc" GEN_PB ${PROTO})
    string(REGEX REPLACE "\\.proto$" "_proto" GEN_TARGET ${PROTO})
    add_custom_command(
            OUTPUT ${PROTO_DIR}/${GEN_PB_HEADER} ${PROTO_DIR}/${GEN_PB}
            COMMAND "${protoc_EXECUTABLE}" --cpp_out=${PROTO_DIR} ${PROTO}
            DEPENDS protoc
            WORKING_DIRECTORY ${PROTO_DIR})

    add_library(${GEN_TARGET}
            "${PROTO_DIR}/${GEN_PB_HEADER}"
            "${PROTO_DIR}/${GEN_PB}")
    target_include_directories(${GEN_TARGET} PUBLIC
            ${protobuf_INCLUDE_DIRS}
            ${PROTO_DIR}
            )
    target_link_libraries(${GEN_TARGET}
            ${protobuf_LIBRARIES}
            )

    set_property(TARGET protobuf APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${GEN_TARGET})
    add_dependencies(protobuf ${GEN_TARGET})
endfunction()

function(compile_proto_to_grpc_cpp PROTO)
    string(REGEX REPLACE "\\.proto$" ".pb.h" GEN_PB_HEADER ${PROTO})
    string(REGEX REPLACE "\\.proto$" ".pb.cc" GEN_PB ${PROTO})
    string(REGEX REPLACE "\\.proto$" ".grpc.pb.h" GEN_GRPC_PB_HEADER ${PROTO})
    string(REGEX REPLACE "\\.proto$" ".grpc.pb.cc" GEN_GRPC_PB ${PROTO})
    string(REGEX REPLACE "\\.proto$" "_proto" GEN_TARGET ${PROTO})
    message(STATUS ${PROTO} - ${GEN_TARGET})
    add_custom_command(
            OUTPUT ${PROTO_DIR}/${GEN_PB_HEADER} ${PROTO_DIR}/${GEN_PB} ${PROTO_DIR}/${GEN_GRPC_PB_HEADER} ${PROTO_DIR}/${GEN_GRPC_PB}
            COMMAND "${protoc_EXECUTABLE}" --cpp_out=${PROTO_DIR} ${PROTO}
            COMMAND "${protoc_EXECUTABLE}" --grpc_out=${PROTO_DIR} --plugin=protoc-gen-grpc="${grpc_CPP_PLUGIN}" ${PROTO}
            DEPENDS grpc_cpp_plugin
            WORKING_DIRECTORY ${PROTO_DIR})

    add_library(${GEN_TARGET}
            "${PROTO_DIR}/${GEN_PB_HEADER}"
            "${PROTO_DIR}/${GEN_PB}"
            "${PROTO_DIR}/${GEN_GRPC_PB_HEADER}"
            "${PROTO_DIR}/${GEN_GRPC_PB}")
    target_include_directories(${GEN_TARGET} PUBLIC
            ${protobuf_INCLUDE_DIRS}
            ${grpc_INCLUDE_DIR}
            ${PROTO_DIR}
            )
    target_link_libraries(${GEN_TARGET}
            ${protobuf_LIBRARIES}
            ${grpc_LIBRARY}
            )

#    set_property(TARGET protobuf APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${GEN_TARGET})
#    set_property(TARGET grpc APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${GEN_TARGET})
#    set_property(TARGET grpc++ APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${GEN_TARGET})
#    add_dependencies(protobuf ${GEN_TARGET})
#    add_dependencies(grpc ${GEN_TARGET})
endfunction()