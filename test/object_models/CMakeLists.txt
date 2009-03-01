add_subdirectory(ffi)

foreach(TEST object_test map_test args_test meta_struct_test)
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries (${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  add_test(object_models::${TEST} ${TEST})
endforeach()

