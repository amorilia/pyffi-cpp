project(ANTLR3C)

# configuration

include(CheckIncludeFile)
check_include_file(sys/malloc.h HAVE_SYS_MALLOC_H)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(stdarg.h HAVE_STDARG_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(ctype.h HAVE_CTYPE_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(netinet/tcp.h HAVE_NETINET_TCP_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(socket.h HAVE_SOCKET_H)
check_include_file(apra/nameser.h HAVE_ARPA_NAMESER_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(resolv.h HAVE_RESOLV_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strdup.h HAVE_STRDUP_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)

include(CheckTypeSize)
set(CMAKE_EXTRA_INCLUDE_FILES inttypes.h)
check_type_size(intptr_t HAVE_INTPTR_T)
check_type_size(uintptr_t HAVE_UINTPTR_T)
set(CMAKE_EXTRA_INCLUDE_FILES)

include(CheckFunctionExists)
check_function_exists(memmove HAVE_MEMMOVE)
check_function_exists(memset HAVE_MEMSET)
# todo: HAVE_ACCEPT

# check 64 bit
check_type_size(void* VOID_PTR_SIZE)
if(VOID_PTR_SIZE EQUAL 8)
  set(ANTLR3_USE_64BIT 1)
endif(VOID_PTR_SIZE EQUAL 8)

configure_file(${ANTLR3C_SOURCE_DIR}/antlr3config.h.cmake ${ANTLR3C_BINARY_DIR}/antlr3config.h)

# includes and sources

# include binary directory so antlr3config.h is found
include_directories(${ANTLR3C_SOURCE_DIR}/include ${ANTLR3C_BINARY_DIR})

set(antlr3c_LIB_SRCS
    src/antlr3baserecognizer.c
    src/antlr3basetree.c
    src/antlr3basetreeadaptor.c
    src/antlr3bitset.c
    src/antlr3collections.c
    src/antlr3commontoken.c
    src/antlr3commontree.c
    src/antlr3commontreeadaptor.c
    src/antlr3commontreenodestream.c
    src/antlr3convertutf.c
    src/antlr3cyclicdfa.c
    src/antlr3debughandlers.c
    src/antlr3encodings.c
    src/antlr3exception.c
    src/antlr3filestream.c
    src/antlr3inputstream.c
    src/antlr3intstream.c
    src/antlr3lexer.c
    src/antlr3parser.c
    src/antlr3rewritestreams.c
    src/antlr3string.c
    src/antlr3stringstream.c
    src/antlr3tokenstream.c
    src/antlr3treeparser.c
    src/antlr3ucs2inputstream.c)

add_library(antlr3c STATIC ${antlr3c_LIB_SRCS})
