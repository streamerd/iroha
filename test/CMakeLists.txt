set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)


AddTest(merkle_tree_test merkle_tree.cc)
target_link_libraries(merkle_tree_test PRIVATE merkle_tree)

AddTest(crypto_test crypto_test.cc)
target_link_libraries(crypto_test PRIVATE crypto)

