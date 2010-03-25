# recurse further
add_subdirectory(pyffi)

# compile tools

java_compile(ffi
  ${PYFFI_BINARY_DIR}/FFILexer.class
  ${PYFFI_BINARY_DIR}/FFIParser.class
  ${PYFFI_BINARY_DIR}/FFITreeOpt.class
  ${PYFFI_BINARY_DIR}/FFITreeTemplate.class)

java_compile(xml2ffi
  ${PYFFI_BINARY_DIR}/XMLLexer.class
  ${PYFFI_BINARY_DIR}/XMLParser.class
  ${PYFFI_BINARY_DIR}/FFITreeOpt.class
  ${PYFFI_BINARY_DIR}/FFITreeTemplate.class)

# ensure dependencies are resolved correctly (cmake can't figure this
# out on its own because targets live in different folders)

add_dependencies(JAVA_ffi ANTLR_FFI ANTLR_FFITreeOpt ANTLR_FFITreeTemplate)
add_dependencies(JAVA_xml2ffi ANTLR_XML ANTLR_FFITreeOpt ANTLR_FFITreeTemplate)

