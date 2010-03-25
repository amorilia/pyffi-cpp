# create lexer, parser, and tree walkers

antlr_lexpar(FFI)
antlr_tree(FFITreeOpt)
antlr_tree(FFITreeTemplate)

add_dependencies(ANTLR_FFITreeOpt ANTLR_FFI)
add_dependencies(ANTLR_FFITreeTemplate ANTLR_FFI)
add_dependencies(JAVA_FFILexer ANTLR_FFI)
add_dependencies(JAVA_FFIParser ANTLR_FFI)
add_dependencies(JAVA_FFITreeOpt ANTLR_FFITreeOpt)
add_dependencies(JAVA_FFITreeTemplate ANTLR_FFITreeTemplate)

