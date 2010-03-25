# create lexer, parser, and tree walkers

antlr_lexpar(FFI)
antlr_tree(FFITreeOpt FFI)
antlr_tree(FFITreeTemplate FFI)

# ensure dependencies

add_dependencies(ANTLR_FFITreeOpt ANLTR_FFI)
add_dependencies(ANTLR_FFITreeTemplate ANLTR_FFI)

