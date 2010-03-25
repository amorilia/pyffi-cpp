cmake_minimum_required(VERSION 2.6)
project(PYFFI Java)

find_package(Java REQUIRED)

# compile a java file in the current source directory to a class file in the global binary
# directory (with $CLASSPATH and global binary directory as classpath)
macro(java_compile javafile)
    add_custom_target(JAVA_${javafile}
        DEPENDS ${PYFFI_BINARY_DIR}/${javafile}.class
    )
    add_custom_command(
        OUTPUT ${PYFFI_BINARY_DIR}/${javafile}.class
        DEPENDS ${javafile}.java
        COMMAND ${JAVA_COMPILE} -cp $ENV{CLASSPATH}:${PYFFI_BINARY_DIR} -d ${PYFFI_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${javafile}.java
        COMMENT "Compiling ${javafile}.java"
    )
endmacro(java_compile)

# make lexer and parser in binary directory
# optional argument is extra tokens dependency
# and directory where to find those tokens
macro(antlr_lexpar grammar) # extratokensdir extratokensfile
    add_custom_target(ANTLR_${grammar}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Lexer.java
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Parser.java
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
    )
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Lexer.java
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}Parser.java
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${ARGV1}${ARGV2}
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g -lib ${ARGV1}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating antlr3 lexer and parser from ${grammar}.g"
    )
    java_compile(${grammar}Lexer)
    java_compile(${grammar}Parser)
endmacro(antlr_lexpar)

# make walker from tree grammar
macro(antlr_tree grammar)
    add_custom_target(ANTLR_${grammar}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.java
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
    )
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.java
            ${CMAKE_CURRENT_SOURCE_DIR}/${grammar}.tokens
        DEPENDS
            ${grammar}.g
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating antlr3 tree walker from ${grammar}.g"
    )
    java_compile(${grammar})
endmacro(antlr_tree)

add_subdirectory(src)

# ensure dependencies are resolved correctly (cmake can't figure this
# out on its own because targets live in different folders)

add_dependencies(ANTLR_XML ANLTR_FFI)
add_dependencies(JAVA_ffi JAVA_FFIParser JAVA_FFILexer JAVA_FFITreeOpt JAVA_FFITreeTemplate)
add_dependencies(JAVA_xml2ffi JAVA_XMLParser JAVA_XMLLexer JAVA_FFITreeOpt JAVA_FFITreeTemplate)

add_custom_target(tools ALL)
add_dependencies(tools JAVA_xml2ffi JAVA_ffi)

enable_testing()
add_subdirectory(test)

