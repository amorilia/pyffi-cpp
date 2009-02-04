add_executable(object_test object_test.cpp)
add_executable(args_test args_test.cpp)
target_link_libraries (object_test boost_unit_test_framework) 
target_link_libraries (args_test boost_unit_test_framework) 
