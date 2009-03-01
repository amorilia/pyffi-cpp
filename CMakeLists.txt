cmake_minimum_required(VERSION 2.6)
project(PYFFI)

# find boost
find_package(Boost REQUIRED COMPONENTS python unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# find python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# build external dependencies
add_subdirectory(extern)

# generate lexer and parser source code
# the grammar must be in the same directory as the generated code
# so first copy the grammar file to the build directory
find_package(Java REQUIRED)
add_custom_command(
    OUTPUT
        ${PYFFI_BINARY_DIR}/FFI.g
        ${PYFFI_BINARY_DIR}/FFI.tokens
        ${PYFFI_BINARY_DIR}/FFILexer.c
        ${PYFFI_BINARY_DIR}/FFILexer.h
        ${PYFFI_BINARY_DIR}/FFIParser.c
        ${PYFFI_BINARY_DIR}/FFIParser.h
    DEPENDS ${PYFFI_SOURCE_DIR}/src/object_models/ffi/FFI.g
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/object_models/ffi/FFI.g FFI.g
    COMMAND ${JAVA_RUNTIME} org.antlr.Tool FFI.g
    WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
    COMMENT "Generating antlr3 lexer and parser from FFI.g")

# include pyffi headers
include_directories(${PYFFI_SOURCE_DIR}/include)

# include lexer and parser headers
include_directories(${PYFFI_BINARY_DIR})

# include antlr3 headers
include_directories(${ANTLR3C_SOURCE_DIR}/include ${ANTLR3C_BINARY_DIR})

# build the actual library
add_library(pyffi
    src/object_models/ffi/fileformat.cpp
    ${PYFFI_BINARY_DIR}/FFILexer.c
    ${PYFFI_BINARY_DIR}/FFIParser.c)
target_link_libraries(pyffi antlr3c)

# build the pypyffi toy application
# (eventually this might go into a seperate project?)
add_executable(pypyffi src/pypyffi.cpp)
target_link_libraries(pypyffi ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARY} pyffi)

# build the tests
enable_testing()
add_subdirectory(test)

# build the benchmarks
add_subdirectory(benchmark)

