add_executable(runtest runtest.cpp)
 
find_package(PythonLibs REQUIRED)
find_package(Boost REQUIRED COMPONENTS python unit_test_framework)
include_directories(${PYTHON_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})
target_link_libraries (runtest ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
