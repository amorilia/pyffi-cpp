# create lexer and parser

antlr_lexpar(XML ../ffi/ FFI.tokens)

# ensure dependencies

add_dependencies(ANTLR_XML ANLTR_FFI)

