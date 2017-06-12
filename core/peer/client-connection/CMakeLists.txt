# this is a correct way to build: https://cmake.org/cmake/help/v2.8.12/cmake.html#module:FindProtobuf

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS interface.proto)
PROTOBUF_GENERATE_GRPC_CPP(PROTO_SRCS_GRPC PROTO_HDRS_GRPC interface.proto)

add_library(torii
        torii.h torii.cc
        ${PROTO_SRCS} ${PROTO_HDRS}
        ${PROTO_SRCS_GRPC} ${PROTO_HDRS_GRPC}
        )

target_link_libraries(torii ${PROTOBUF_LIBRARIES} pthread)

get_filename_component(COMPILED_PROTO_INCLUDE_DIR ${PROTO_HDRS} DIRECTORY)
get_filename_component(COMPILED_PROTO_GRPC_INCLUDE_DIR ${PROTO_HDRS_GRPC} DIRECTORY)
include_directories(
        ${COMPILED_PROTO_INCLUDE_DIR}
        ${COMPILED_PROTO_GRPC_INCLUDE_DIR}
)
