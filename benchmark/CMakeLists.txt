add_executable(attribute_access attribute_access.cpp)

find_package(PythonLibs REQUIRED)
find_package(Boost REQUIRED COMPONENTS python)
include_directories(${PYTHON_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})
target_link_libraries (attribute_access ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
