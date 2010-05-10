cmake_minimum_required(VERSION 2.6)
project(PYFFI)

find_package(Java REQUIRED)

# find boost
find_package(Boost 1.43.0 REQUIRED COMPONENTS python unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# find python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# include pyffi headers
include_directories(${PYFFI_SOURCE_DIR}/include)

add_subdirectory(src)

# build the actual library
add_library(pyffi
    src/pyffi/object_models/ffi/file_format.cpp
    src/pyffi/object_models/xml/file_format.cpp
)

# build the tests
enable_testing()
add_subdirectory(test)

# build the benchmarks
add_subdirectory(benchmark)
