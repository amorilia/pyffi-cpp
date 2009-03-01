find_package(Java REQUIRED)
add_custom_command(
    OUTPUT FFI.tokens FFILexer.c FFILexer.h FFIParser.c FFIParser.h
    DEPENDS FFI.g
    COMMAND ${JAVA_RUNTIME} org.antlr.Tool ${CMAKE_CURRENT_SOURCE_DIR}/FFI.g
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
