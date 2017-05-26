set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)

add_executable(merkle_tree_test merkle_tree.cc)
target_link_libraries(merkle_tree_test merkle_tree gtest gtest_main)
add_test(merkle_tree_test merkle_tree_test)