/*

Copyright (c) 2007-2008, Python File Format Interface
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the Python File Format Interface
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "antlr3.h"
#include "FFILexer.h"

BOOST_AUTO_TEST_SUITE(ffi_test_suite)

void create_parser(const std::string & filename) {
	// set up the antlr structures
	pANTLR3_INPUT_STREAM           input;
	pSimpleCalcLexer               lex;
	pANTLR3_COMMON_TOKEN_STREAM    tokens;
	pSimpleCalcParser              parser;

	input  = antlr3AsciiFileStreamNew          ((pANTLR3_UINT8)filename);
	lex    = SimpleCalcLexerNew                (input);
	tokens = antlr3CommonTokenStreamSourceNew  (ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	parser = SimpleCalcParserNew               (tokens);

	parser  ->expr(parser);

	// clean up
	parser ->free(parser);
	tokens ->free(tokens);
	lex    ->free(lex);
	input  ->close(input);
}

BOOST_AUTO_TEST_CASE(type_test) {
	create_parser("test_type.ffi");
}

BOOST_AUTO_TEST_CASE(type_doc_test) {
	create_parser("test_type_doc.ffi");
}

BOOST_AUTO_TEST_CASE(class_test) {
	create_parser("test_class.ffi");
}

BOOST_AUTO_TEST_CASE(parameter_test) {
	create_parser("test_parameter.ffi");
}

BOOST_AUTO_TEST_CASE(kwargs_test) {
	create_parser("test_kwargs.ffi");
}

BOOST_AUTO_TEST_CASE(conditions_test) {
	create_parser("test_conditions.ffi");
}

BOOST_AUTO_TEST_SUITE_END()
