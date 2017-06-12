find_path(uv_INCLUDE_DIRS uv.h)
find_library(uv_LIBRARIES uv)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(uv
        FOUND_VAR uv_FOUND
        REQUIRED_VARS uv_INCLUDE_DIRS uv_LIBRARIES
        FAIL_MESSAGE "libuv is not found"
        )