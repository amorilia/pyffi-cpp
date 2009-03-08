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

#include <string>

#include "antlr3.h"

#include "FFILexer.h"
#include "FFIParser.h"
#include "FFIFileFormat.h"

#include "pyffi/object_models/ffi/file_format.hpp"
#include "pyffi/exceptions.hpp"

namespace pyffi {

namespace object_models {

namespace ffi {

/*
 * Main entry point for lexing, parsing, and walking.
 */

FileFormat::FileFormat(const std::string & filename) {
	// set up the antlr structures
	pANTLR3_INPUT_STREAM input = NULL;
	pFFILexer lex = NULL;
	pANTLR3_COMMON_TOKEN_STREAM tokens = NULL;
	pFFIParser parser = NULL;
	pANTLR3_BASE_TREE ast = NULL;
	pANTLR3_COMMON_TREE_NODE_STREAM nodes = NULL;
	pFFIFileFormat walker = NULL;

	input = antlr3AsciiFileStreamNew((pANTLR3_UINT8)filename.c_str());
	if (input == NULL) {
		throw io_error("Could not open '" + filename + "'.");
	};
	lex = FFILexerNew(input);
	if (lex == NULL) {
		input->close(input);
		throw runtime_error("Could not create lexer for '" + filename + "' (insufficient memory?).");
	};
	tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	if (tokens == NULL) {
		lex->free(lex);
		input->close(input);
		throw runtime_error("Could not create tokens for '" + filename + "' (insufficient memory?).");
	};
	parser = FFIParserNew(tokens);
	if (parser == NULL) {
		tokens->free(tokens);
		lex->free(lex);
		input->close(input);
		throw runtime_error("Could not create parser for '" + filename + "' (insufficient memory?).");
	};
	// parse the file
	ast = parser->ffi(parser).tree;
	// check that parsing succeeded
	if (parser->pParser->rec->state->errorCount > 0) {
		parser->free(parser);
		tokens->free(tokens);
		lex->free(lex);
		input->close(input);
		throw syntax_error("Syntax error while parsing '" + filename + "'.");
	};

	// for debugging
	printf("Abstract syntax tree: \n%s\n\n", ast->toStringTree(ast)->chars);

        nodes = antlr3CommonTreeNodeStreamNewTree(ast, ANTLR3_SIZE_HINT);
	if (nodes == NULL) {
	        parser->free(parser);
		tokens->free(tokens);
		lex->free(lex);
		input->close(input);
		throw runtime_error("Could not create nodes for '" + filename + "' (insufficient memory?).");
	};
        walker = FFIFileFormatNew(nodes);
	if (walker == NULL) {
                nodes->free(nodes);
	        parser->free(parser);
		tokens->free(tokens);
		lex->free(lex);
		input->close(input);
		throw runtime_error("Could not create walker for '" + filename + "' (insufficient memory?).");
	};
        walker->ffi(walker, this);

	// release memory
        if (walker) walker->free(walker);
        if (nodes) nodes->free(nodes);
	if (parser) parser->free(parser);
	if (tokens) tokens->free(tokens);
	if (lex) lex->free(lex);
	if (input) input->close(input);
};

}; // namespace ffi

}; // namespace object_models

}; // namespace pyffi
