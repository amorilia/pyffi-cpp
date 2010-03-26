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
# and directory where to find those tokens
macro(antlr_lexpar grammar) # extratokensdir extratokensfile
    add_custom_target(ANTLR_${grammar}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Lexer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Lexer.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Parser.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Parser.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
    )
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Lexer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Lexer.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Parser.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Parser.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${ARGV1}${ARGV2}
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g -lib ${ARGV1}
        # change extension of files to force cmake to use C++ compiler
        COMMAND ${CMAKE_COMMAND} -E copy ${grammar}Lexer.c ${grammar}Lexer.cpp
        COMMAND ${CMAKE_COMMAND} -E copy ${grammar}Parser.c ${grammar}Parser.cpp
        COMMAND ${CMAKE_COMMAND} -E remove ${grammar}Lexer.c
        COMMAND ${CMAKE_COMMAND} -E remove ${grammar}Parser.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating antlr3 lexer and parser from ${grammar}.g"
    )
endmacro(antlr_lexpar)

# make walker from tree grammar
macro(antlr_tree grammar)
    add_custom_target(ANTLR_${grammar}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
    )
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
        DEPENDS
            ${grammar}.g
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        # change extension of files to force cmake to use C++ compiler
        COMMAND ${CMAKE_COMMAND} -E copy ${grammar}.c ${grammar}.cpp
        COMMAND ${CMAKE_COMMAND} -E remove ${grammar}.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating antlr3 tree walker from ${grammar}.g"
    )
endmacro(antlr_tree)

# build external dependencies
add_subdirectory(extern)

# include pyffi headers
include_directories(${PYFFI_SOURCE_DIR}/include)

# include lexer and parser headers
include_directories(${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi)
include_directories(${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml)

# include antlr3 headers
include_directories(${ANTLR3C_SOURCE_DIR}/include ${ANTLR3C_BINARY_DIR})

add_subdirectory(src)

# ensure dependencies are resolved correctly (cmake can't figure this
# out on its own because targets live in different folders)

add_dependencies(ANTLR_XML ANLTR_FFI)
add_dependencies(ANTLR_FFITreeOpt ANTLR_FFI)
add_dependencies(ANTLR_FFIFileFormat ANTLR_FFI)

# build the actual library
add_library(pyffi
    src/pyffi/object_models/ffi/file_format.cpp
    src/pyffi/object_models/xml/file_format.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFILexer.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFIParser.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFIFileFormat.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XMLLexer.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XMLParser.cpp)
set_source_files_properties(
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFILexer.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFIParser.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFIFileFormat.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XMLLexer.cpp
    ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XMLParser.cpp
    PROPERTIES GENERATED 1)
target_link_libraries(pyffi antlr3c)
add_dependencies(pyffi ANTLR_XML ANTLR_FFI ANTLR_FFITreeOpt ANTLR_FFIFileFormat)

# build the tests
enable_testing()
add_subdirectory(test)

# build the benchmarks
add_subdirectory(benchmark)
