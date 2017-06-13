# this is a correct way to build PROTO: https://cmake.org/cmake/help/v2.8.12/cmake.html#module:FindProtobuf


# this folder contains the code, which is responsible for CLIENT -> PEER communication

COMPILE_PROTO     (S1 H1 ${PROTO_DIR}/api.proto)
COMPILE_PROTO     (S2 H2 ${PROTO_DIR}/api_service.proto)
COMPILE_GRPC_PROTO(S3 H3 ${PROTO_DIR}/api_service.proto)

add_library(async_server
        async_server.h async_server.cc
        ${H1} ${S1} # compiled api.pb.{h,cc}
        ${H2} ${S2} # compiled api_service.pb.{h,cc}
        ${H3} ${S3} # compiled api_service.grpc.pb.{h,cc}
        )


get_filename_component(COMPILED_HDRS_DIR ${H1} DIRECTORY)
target_include_directories(async_server PUBLIC ${COMPILED_HDRS_DIR})

target_link_libraries(async_server PUBLIC
        pthread
        grpc++
        gpr
        ${PROTOBUF_LIBRARIES}
        )
