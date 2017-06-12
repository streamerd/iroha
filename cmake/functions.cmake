# Creates test "test_name", with "SOURCES" (use string as second argument)
function(AddTest test_name SOURCES)
    add_executable(${test_name} ${SOURCES})
    target_link_libraries(${test_name} PRIVATE gtest gtest_main)
    add_test(
            NAME ${test_name}
            COMMAND $<TARGET_FILE:${test_name}>
    )
endfunction()