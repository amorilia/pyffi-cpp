# run the tests
foreach(TEST ffi_test)
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries (${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pyffi)
    # the ffi files reside in the source directory
    # so we run the actual test from that directory
    # (the default is from the binary directory)
    add_test(pyffi::object_models::ffi::${TEST} ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
endforeach()
