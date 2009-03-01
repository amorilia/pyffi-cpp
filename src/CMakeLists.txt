add_subdirectory(object_models)

add_executable(pyffi pyffi.cpp)

find_package(PythonLibs REQUIRED)
find_package(Boost REQUIRED COMPONENTS python)
include_directories(${PYTHON_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})
target_link_libraries (pyffi ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
