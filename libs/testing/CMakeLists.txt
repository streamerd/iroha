COMPILE_PROTO(S1 H1 ${PROTO_DIR}/api.proto)

add_library(generator STATIC
    generator.cc generator.h
    tx_generator.h
    ${S1} ${H1} # compiled api.pb.{cc,h}
    )

get_filename_component(COMPILED_HDRS_DIR ${H1} DIRECTORY)
target_include_directories(generator PUBLIC ${COMPILED_HDRS_DIR})

target_link_libraries(generator PUBLIC
    ${PROTOBUF_LIBRARIES}
    crypto
    )