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

#include "pyffi/object_models/ffi/fileformat.hpp"
#include "pyffi/exceptions.hpp"

#include "antlr3.h"
#include "FFILexer.h"
#include "FFIParser.h"

namespace pyffi {

void _test_parser(const std::string & filename) {
	// set up the antlr structures
	pANTLR3_INPUT_STREAM           input;
	pFFILexer               lex;
	pANTLR3_COMMON_TOKEN_STREAM    tokens;
	pFFIParser              parser;

	input  = antlr3AsciiFileStreamNew          ((pANTLR3_UINT8)filename.c_str());
	if (input == NULL) {
		throw io_error("Could not open '" + filename + "'.");
	};
	/*
	lex    = FFILexerNew                (input);
	tokens = antlr3CommonTokenStreamSourceNew  (ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	parser = FFIParserNew               (tokens);

	//parser  ->ffi(parser);

	// clean up
	parser ->free(parser);
	tokens ->free(tokens);
	lex    ->free(lex);
	*/
	input  ->close(input);
}

}
