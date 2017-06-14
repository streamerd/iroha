include_directories(.)

# connection with CLIENT -> PEER
add_subdirectory(api)
# consensus algorithm
add_subdirectory(consensus)
# connection PEER -> STORAGE
add_subdirectory(storage)

add_subdirectory(main)


