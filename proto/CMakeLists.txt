COMPILE_PROTO     (S1 H1 ${PROTO_DIR}/api.proto)
COMPILE_PROTO     (S2 H2 ${PROTO_DIR}/api_service.proto)
COMPILE_GRPC_PROTO(S3 H3 ${PROTO_DIR}/api_service.proto)

get_filename_component(PROTO_COMPILED_DIR ${H1} DIRECTORY)
include_directories(${PROTO_COMPILED_DIR})

