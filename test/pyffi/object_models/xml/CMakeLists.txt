# run the tests
foreach(TEST test_basic test_enum test_full test_header test_minimal test_version)
    add_test(pyffi::object_models::xml::${TEST} ${CMAKE_COMMAND} -E chdir ${PYFFI_BINARY_DIR} ${JAVA_RUNTIME} -cp $ENV{CLASSPATH}:${PYFFI_BINARY_DIR} xml2ffi ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}.xml ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}.ffi)
endforeach()
