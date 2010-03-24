cmake_minimum_required(VERSION 2.6)
project(PYFFI)

find_package(Java REQUIRED)

# compile a java file in the binary directory to a class file in the binary
# directory (with $CLASSPATH and binary directory as classpath)
function(java_compile javafile)
    add_custom_command(
        OUTPUT ${javafile}.class
	DEPENDS ${javafile}.java ${ARGN}
	COMMAND ${JAVA_COMPILE} -cp $ENV{CLASSPATH}:${PYFFI_BINARY_DIR} -d ${PYFFI_BINARY_DIR} ${javafile}.java
	WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
    )
    add_custom_target(${javafile}_class
        DEPENDS ${javafile}.class
    )
endfunction(java_compile)

# make lexer and parser in binary directory
# optional argument is extra tokens dependency
function(antlr_lexpar grammar)
    add_custom_command(
        OUTPUT
            ${grammar}Lexer.java
            ${grammar}Parser.java
            ${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${ARGN}
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
        COMMENT "Generating antlr3 lexer and parser from ${grammar}.g"
    )
    java_compile(${grammar}Lexer)
    java_compile(${grammar}Parser)
endfunction(antlr_lexpar)

# make walker from tree grammar
function(antlr_tree grammar basegrammar)
    add_custom_command(
        OUTPUT
            ${grammar}.java
            ${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${basegrammar}Lexer.java
            ${basegrammar}Parser.java
            ${basegrammar}.tokens
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
        COMMENT "Generating antlr3 tree walker from ${grammar}.g"
    )
    java_compile(${grammar}
        ${basegrammar}Lexer.class
        ${basegrammar}Parser.class)
endfunction(antlr_tree)

antlr_lexpar(FFI)
antlr_lexpar(XML FFI.tokens)
antlr_tree(FFITreeOpt FFI)
antlr_tree(FFITreeTemplate FFI)
java_compile(xml2ffi
  XMLLexer.class
  XMLParser.class
  FFITreeOpt.class
  FFITreeTemplate.class)
java_compile(ffi
  FFILexer.class
  FFIParser.class
  FFITreeOpt.class
  FFITreeTemplate.class)

# copy relevant files to binary directory
add_custom_command(
    OUTPUT
       ${PYFFI_BINARY_DIR}/FFI.g
       ${PYFFI_BINARY_DIR}/FFITreeOpt.g
       ${PYFFI_BINARY_DIR}/FFITreeTemplate.g
       ${PYFFI_BINARY_DIR}/ffi.stg
       ${PYFFI_BINARY_DIR}/XML.g
       ${PYFFI_BINARY_DIR}/xml2ffi.java
       ${PYFFI_BINARY_DIR}/ffi.java
    DEPENDS
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFI.g
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFITreeOpt.g
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFITreeTemplate.g
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/ffi.stg
       ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XML.g
       ${PYFFI_SOURCE_DIR}/src/xml2ffi.java
       ${PYFFI_SOURCE_DIR}/src/ffi.java
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFI.g ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFITreeOpt.g ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/FFITreeTemplate.g ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/ffi/ffi.stg ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/pyffi/object_models/xml/XML.g ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/xml2ffi.java ${PYFFI_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PYFFI_SOURCE_DIR}/src/ffi.java ${PYFFI_BINARY_DIR}
)

add_custom_target(translate ALL
    DEPENDS
        xml2ffi.class
        ffi.class
        ${PYFFI_BINARY_DIR}/ffi.stg
#    COMMAND ${CMAKE_COMMAND} -E make_directory xml2ffi_test
#    COMMAND ${JAVA_RUNTIME} -cp $ENV{CLASSPATH}:${PYFFI_BINARY_DIR} xml2ffi ${PYFFI_SOURCE_DIR}/test/ xml2ffi_test/
    WORKING_DIRECTORY ${PYFFI_BINARY_DIR}
)

enable_testing()
add_subdirectory(test)
