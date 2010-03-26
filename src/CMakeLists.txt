# recurse further
add_subdirectory(pyffi)

# compile tools

java_compile(FFITool)
java_compile(XMLTool)
java_compile(ffi2ffi)
java_compile(xml2ffi)
