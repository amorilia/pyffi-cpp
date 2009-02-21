add_executable(pyffi pyffi.cpp)

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})
target_link_libraries (pyffi ${PYTHON_LIBRARIES})
target_link_libraries (pyffi boost_python) 
