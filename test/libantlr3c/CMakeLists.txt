foreach(TEST antlr3tokenqueue_test)
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries (${TEST} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} antlr3c)
  add_test(libantlr3c::${TEST} ${TEST})
endforeach()

