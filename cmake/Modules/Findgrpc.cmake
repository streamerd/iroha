find_path(grpc_INCLUDE_DIR grpc/grpc.h)
find_path(grpcpp_INCLUDE_DIR grpc++/grpc++.h)
find_library(grpc_LIB grpc)
find_library(grpcpp_LIB grpc++)
find_library(gpr_LIB gpr)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(grpc
        FOUND_VAR grpc_FOUND
        REQUIRED_VARS grpc_INCLUDE_DIR grpcpp_INCLUDE_DIR grpc_LIB grpcpp_LIB gpr_LIB
        FAIL_MESSAGE "grpc is not found"
        )


find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin) # Get full path to plugin

function(PROTOBUF_GENERATE_GRPC_CPP SRCS HDRS)
    if(NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_GRPC_CPP() called without any proto files")
        return()
    endif()

    if(PROTOBUF_GENERATE_CPP_APPEND_PATH) # This variable is common for all types of output.
        # Create an include path for each file specified
        foreach(FIL ${ARGN})
            get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
            get_filename_component(ABS_PATH ${ABS_FIL} PATH)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    else()
        set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if(DEFINED PROTOBUF_IMPORT_DIRS)
        foreach(DIR ${Protobuf_IMPORT_DIRS})
            get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    endif()

    set(${SRCS})
    set(${HDRS})
    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)

        list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")
        list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")

        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc"
                "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h"
                COMMAND  ${Protobuf_PROTOC_EXECUTABLE}
                ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
                --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
                ${_protobuf_include_path} ${ABS_FIL}
                DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE}
                COMMENT "Running gRPC C++ protocol buffer compiler on ${FIL}"
                VERBATIM)
    endforeach()

    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()