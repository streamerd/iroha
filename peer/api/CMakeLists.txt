# this is a correct way to build: https://cmake.org/cmake/help/v2.8.12/cmake.html#module:FindProtobuf

add_library(torii
        torii.h torii.cc
        ${PROTO_SRCS} ${PROTO_HDRS}
        ${PROTO_SRCS_GRPC} ${PROTO_HDRS_GRPC}
        )

target_link_libraries(torii  pthread)


include_directories(
        ${COMPILED_PROTO_INCLUDE_DIR}
        ${COMPILED_PROTO_GRPC_INCLUDE_DIR}
)


add_library(grpcServerRunner grpcServerRunner.cc)
target_link_libraries(grpcServerRunner torii grpc grpc++)

add_library(toriiImpl toriiImpl.cc)
target_link_libraries(toriiImpl torii)
