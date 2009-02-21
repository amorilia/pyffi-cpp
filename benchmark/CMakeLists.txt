add_executable(attribute_access attribute_access.cpp)

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})
target_link_libraries (attribute_access ${PYTHON_LIBRARIES})
target_link_libraries (attribute_access boost_python) 
