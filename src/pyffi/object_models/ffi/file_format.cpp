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

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>

#include "pyffi/object_models/ffi/file_format.hpp"
#include "pyffi/exceptions.hpp"

namespace pyffi
{

namespace object_models
{

namespace ffi
{

template <typename Iterator>
struct ffi_parser : boost::spirit::qi::grammar<Iterator, void(), boost::spirit::qi::ascii::space_type> {
    boost::spirit::qi::rule<Iterator, void(), boost::spirit::qi::ascii::space_type> start;
    boost::spirit::qi::rule<Iterator, void()> comment;

    ffi_parser()
	    : ffi_parser::base_type(start) {
	using boost::phoenix::construct;
	using boost::phoenix::val;
	using boost::spirit::qi::space;
	using boost::spirit::qi::char_;
	using boost::spirit::qi::eol;
	using boost::spirit::qi::lexeme;
	using boost::spirit::qi::lit;
	using boost::spirit::qi::on_error;
	using boost::spirit::qi::fail;
	using boost::spirit::qi::_1;
	using boost::spirit::qi::_2;
	using boost::spirit::qi::_3;
	using boost::spirit::qi::_4;

	start = *comment >> lit("fileformat");

	// the comment rule has no skipper, we need to specify whitespace
	comment = char_('#')
	          >> *(space - eol)
	          >> (*(char_ - space) % (space - eol))
	          >> eol;

	start.name("fileformat");
	comment.name("#comment");

	on_error<fail>(
	    start,
	    std::cout
	    << val("Error! Expecting ")
	    << _4                               // what failed?
	    << val(" here: \"")
	    << construct<std::string>(_3, _2)   // iterators to error-pos, end
	    << val("\"")
	    << std::endl
	);
};
};

/*
 * Main entry point for parsing an ffi file.
 */

FileFormat::FileFormat(const std::string & filename)
{
	// open file, disable skipping of whitespace
	std::ifstream in(filename.c_str());
	if (in.fail()) {
		throw io_error("Could not open '" + filename + "'.");
	}
	in.unsetf(std::ios::skipws);

	// wrap istream into iterator
	boost::spirit::istream_iterator first(in);
	boost::spirit::istream_iterator last;

	// create parser
	ffi_parser<boost::spirit::istream_iterator> parser;

	// use iterator to parse file data
	bool r = boost::spirit::qi::phrase_parse(
	             first,
	             last,
	             parser,
	             boost::spirit::qi::ascii::space);

	// fail if we did not get a full match
	if (!r) // || first != last)
		throw syntax_error("Syntax error while parsing '" + filename + "'.");
};

}; // namespace ffi

}; // namespace object_models

}; // namespace pyffi
