# create lexer and parser

antlr_lexpar(XML ../ffi/ FFI.tokens)

add_dependencies(JAVA_XMLLexer ANTLR_XML)
add_dependencies(JAVA_XMLParser ANTLR_XML)

# not sure why we need to add this here as well
# dependency from the main CMakeLists.txt does not work
add_dependencies(ANTLR_XML ANTLR_FFI)

