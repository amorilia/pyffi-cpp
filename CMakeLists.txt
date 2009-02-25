cmake_minimum_required(VERSION 2.6)
project(PYFFI)
include_directories(${PYFFI_SOURCE_DIR}/include)
enable_testing()

add_subdirectory(test)
add_subdirectory(src)
add_subdirectory(benchmark)

