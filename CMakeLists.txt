cmake_minimum_required(VERSION 2.6)
project(PYFFI Java)

find_package(Java REQUIRED)

# compile a java file in the current source directory to a class file in the global binary
# directory (with $CLASSPATH and global binary directory as classpath)
macro(java_compile javafile)
    add_custom_target(JAVA_${javafile} ALL
        DEPENDS ${PYFFI_BINARY_DIR}/${javafile}.class
        COMMENT "JAVA_${javafile}"
    )
    add_custom_command(
        OUTPUT ${PYFFI_BINARY_DIR}/${javafile}.class
        DEPENDS ${javafile}.java ${ARGN}
        COMMAND ${JAVA_COMPILE} -cp $ENV{CLASSPATH}:${PYFFI_BINARY_DIR} -d ${PYFFI_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${javafile}.java
        COMMENT "Compiling ${javafile}.java"
    )
endmacro(java_compile)

# make lexer and parser in binary directory
# optional argument is extra tokens dependency
# and directory where to find those tokens
macro(antlr_lexpar grammar) # extratokensdir extratokensfile
    add_custom_target(ANTLR_${grammar} ALL
        DEPENDS
            ${PYFFI_BINARY_DIR}/${grammar}Lexer.class
            ${PYFFI_BINARY_DIR}/${grammar}Parser.class
        COMMENT "ANTLR_${grammar}"
    )
    add_custom_command(
        OUTPUT
            ${grammar}Lexer.java
            ${grammar}Parser.java
            ${grammar}.tokens
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
macro(antlr_tree grammar basegrammar)
    add_custom_target(ANTLR_${grammar} ALL
        DEPENDS
            ${PYFFI_BINARY_DIR}/${grammar}.class
    )
    add_custom_command(
        OUTPUT
            ${grammar}.java
            ${grammar}.tokens
        DEPENDS
            ${grammar}.g
            ${PYFFI_BINARY_DIR}/${basegrammar}Lexer.class
            ${PYFFI_BINARY_DIR}/${basegrammar}Parser.class
        # generate lexer and parser
        COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${grammar}.g
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating antlr3 tree walker from ${grammar}.g"
    )
    java_compile(${grammar} ${PYFFI_BINARY_DIR}/${basegrammar}Lexer.class ${PYFFI_BINARY_DIR}/${basegrammar}Parser.class)
endmacro(antlr_tree)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
