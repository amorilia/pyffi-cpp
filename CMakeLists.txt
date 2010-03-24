cmake_minimum_required(VERSION 2.6)
project(PYFFI)

find_package(Java REQUIRED)

# find boost
find_package(Boost REQUIRED COMPONENTS python unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# find python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# make lexer and parser in binary directory
# optional argument is extra tokens dependency
function(antlr_lexpar grammar)
    add_custom_command(
        OUTPUT
            ${grammar}Lexer.cpp
            ${grammar}Lexer.h
            ${grammar}Parser.cpp
            ${grammar}Parser.h
            ${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${ARGN}
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        # change extension of files to force cmake to use C++ compiler
        COMMAND ${CMAKE_COMMAND} -E copy ${grammar}Lexer.c ${grammar}Lexer.cpp
        COMMAND ${CMAKE_COMMAND} -E copy ${grammar}Parser.c ${grammar}Parser.cpp
        WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
        COMMENT "Generating antlr3 lexer and parser from ${grammar}.g"
    )
endfunction(antlr_lexpar)

# make walker from tree grammar
function(antlr_tree grammar basegrammar)
    add_custom_command(
        OUTPUT
            ${grammar}.cpp
            ${grammar}.h
            ${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${basegrammar}Lexer.h
            ${basegrammar}Parser.h
            ${basegrammar}.tokens
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        # change extension of files to force cmake to use C++ compiler
        COMMAND ${CMAKE_COMMAND} -E copy ${grammar}.c ${grammar}.cpp
        WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
        COMMENT "Generating antlr3 tree walker from ${grammar}.g"
    )
endfunction(antlr_tree)

# build external dependencies
add_subdirectory(extern)

antlr_lexpar(FFI)
antlr_lexpar(XML FFI.tokens)
antlr_tree(FFITreeOpt FFI)
antlr_tree(FFITreeTemplate FFI)

# copy relevant files to binary directory
add_custom_command(
    OUTPUT
       ${PYFFI_BINARY_DIR}/FFI.g
       ${PYFFI_BINARY_DIR}/FFITreeOpt.g
       ${PYFFI_BINARY_DIR}/XML.g
    DEPENDS
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFI.g
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFITreeOpt.g
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XML.g
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFI.g ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFITreeOpt.g ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XML.g ${PYFFI_BINARY_DIR}
)

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
