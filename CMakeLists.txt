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
        ${PYFFI_BINARY_DIR}/FFILexer.cpp
        ${PYFFI_BINARY_DIR}/FFILexer.h
        ${PYFFI_BINARY_DIR}/FFIParser.cpp
        ${PYFFI_BINARY_DIR}/FFIParser.h
        ${PYFFI_BINARY_DIR}/XMLLexer.cpp
        ${PYFFI_BINARY_DIR}/XMLLexer.h
        ${PYFFI_BINARY_DIR}/XMLParser.cpp
        ${PYFFI_BINARY_DIR}/XMLParser.h
        ${PYFFI_BINARY_DIR}/FFIFileFormat.cpp
        ${PYFFI_BINARY_DIR}/FFIFileFormat.h
    DEPENDS
        ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFI.g
        ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XML.g
        ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFIFileFormat.g
    # copy grammar to build dir
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFI.g FFI.g
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XML.g XML.g
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFIFileFormat.g FFIFileFormat.g
    # generate lexer, parser, and walker
    COMMAND ${JAVA_RUNTIME} org.antlr.Tool FFI.g
    COMMAND ${JAVA_RUNTIME} org.antlr.Tool XML.g
    COMMAND ${JAVA_RUNTIME} org.antlr.Tool FFIFileFormat.g
    # change extension of files to force cmake to use C++ compiler
    COMMAND ${CMAKE_COMMAND} -E copy FFILexer.c FFILexer.cpp
    COMMAND ${CMAKE_COMMAND} -E copy FFIParser.c FFIParser.cpp
    COMMAND ${CMAKE_COMMAND} -E copy XMLLexer.c XMLLexer.cpp
    COMMAND ${CMAKE_COMMAND} -E copy XMLParser.c XMLParser.cpp
    COMMAND ${CMAKE_COMMAND} -E copy FFIFileFormat.c FFIFileFormat.cpp
    # clean up
    COMMAND ${CMAKE_COMMAND} -E remove FFILexer.c FFIParser.c XMLLexer.c XMLParser.c FFIFileFormat.c FFI.g XML.g FFIFileFormat.g FFI.tokens FFIFileFormat.tokens
    WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
    COMMENT "Generating antlr3 lexer and parser from FFI.g XML.g FFIFileFormat.g")

# include pyffi headers
include_directories(${PYFFI_SOURCE_DIR}/include)

# include lexer and parser headers
include_directories(${PYFFI_BINARY_DIR})

# include antlr3 headers
include_directories(${ANTLR3C_SOURCE_DIR}/include ${ANTLR3C_BINARY_DIR})

# build the actual library
add_library(pyffi
    src/pyffi/object_models/ffi/file_format.cpp
    src/pyffi/object_models/xml/file_format.cpp
    ${PYFFI_BINARY_DIR}/FFILexer.cpp
    ${PYFFI_BINARY_DIR}/FFIParser.cpp
    ${PYFFI_BINARY_DIR}/XMLLexer.cpp
    ${PYFFI_BINARY_DIR}/XMLParser.cpp
    ${PYFFI_BINARY_DIR}/FFIFileFormat.cpp)
target_link_libraries(pyffi antlr3c)

# build the tests
enable_testing()
add_subdirectory(test)

# build the benchmarks
add_subdirectory(benchmark)

# build other application(s) and python wrapper
add_subdirectory(src)
