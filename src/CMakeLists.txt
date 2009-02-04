include_directories(/usr/include/python2.5)
add_executable(pyffi pyffi.cpp)
target_link_libraries (pyffi python2.5)
target_link_libraries (pyffi boost_python) 
