# here will be the code, which is responsible for connection to ametsuchi
message(STATUS "Storage")

add_library(storage
    storage.cc storage.h
    )

target_link_libraries(storage PUBLIC
    ametsuchi
    )