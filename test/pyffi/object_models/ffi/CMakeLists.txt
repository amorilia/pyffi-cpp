# run the tests
foreach(TEST test_array test_class test_conditions test_enum test_expression test_kwargs test_multi_name test_name test_parameter test_type_doc test_type)
    add_test(pyffi::object_models::ffi::${TEST} ${CMAKE_COMMAND} -E chdir ${PYFFI_BINARY_DIR} ${JAVA_RUNTIME} -cp $ENV{CLASSPATH}:${PYFFI_BINARY_DIR} ffi2ffi ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/ffi.stg ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}.ffi)
endforeach()
