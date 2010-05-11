add_subdirectory(ffi)
add_subdirectory(xml)

foreach(TEST object_test map_test class_test instance_test expression_test)
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries (${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pyffi)
  add_test(pyffi::object_models::${TEST} ${TEST})
endforeach()

