COMPILE_PROTO     (S1 H1 ${PROTO_DIR}/api.proto)
COMPILE_PROTO     (S2 H2 ${PROTO_DIR}/api_service.proto)
COMPILE_GRPC_PROTO(S3 H3 ${PROTO_DIR}/api_service.proto)

add_executable(client
        client.h
        client.cc
        ${H1} ${S1} # compiled api.pb.{h,cc}
        ${H2} ${S2} # compiled api_service.pb.{h,cc}
        ${H3} ${S3} # compiled api_service.grpc.pb.{h,cc}
        )

get_filename_component(COMPILED_HDRS_DIR ${H1} DIRECTORY)
target_include_directories(client PUBLIC ${COMPILED_HDRS_DIR})

target_link_libraries(client PUBLIC
        ${PROTOBUF_LIBRARIES}
        grpc++
        gpr
        optional
        crypto
)