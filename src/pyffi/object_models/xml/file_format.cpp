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

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <fstream>

#include "pyffi/object_models/xml/file_format.hpp"
#include "pyffi/exceptions.hpp"

namespace pyffi
{

namespace object_models
{

namespace xml
{

namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;

template <typename Iterator>
struct xml_skipper : qi::grammar<Iterator> {
	qi::rule<Iterator> start;

	xml_skipper()
		: xml_skipper::base_type(start) {
		start =
		    qi::space
		    //[std::cout << phoenix::val("SPACE") << std::endl]
		    |
		    qi::lit("<!--")
		    //[std::cout << phoenix::val("XMLCOMMENT=")]
		    >> *(!qi::lit("-->")
		         >> qi::char_
		         //[std::cout << qi::_1]
		        )
		    >> qi::lit("-->");
	};
};

template <typename Iterator, typename Skipper = qi::grammar<Iterator> >
struct xml_parser : qi::grammar<Iterator, Skipper> {
	qi::rule<Iterator, Skipper> start;
	qi::rule<Iterator, Skipper> comment;
	qi::rule<Iterator, Skipper> version;
	qi::rule<Iterator, unsigned(), Skipper> num_attribute;

	xml_parser()
		: xml_parser::base_type(start) {
		using phoenix::construct;
		using phoenix::val;
		using qi::char_;
		using qi::eoi;
		using qi::eps;
		using qi::fail;
		using qi::graph;
		using qi::lexeme;
		using qi::lit;
		using qi::on_error;
		using qi::space;
		using qi::uint_;
		using qi::_1;
		using qi::_2;
		using qi::_3;
		using qi::_4;
		using qi::_val;

		start =
		    -(lit("<?xml")[std::cout << val("XML") << std::endl]
		      >> *(!lit("?>") >> char_)
		      >> lit("?>"))
		    >> -(lit("<!DOCTYPE")[std::cout << val("DOCTYPE") << std::endl]
		         >> lit("niftoolsxml")
		         >> lit(">"))
		    >> lit("<niftoolsxml")[std::cout << val("NIFTOOLSXML") << std::endl]
		    >> -(lit("version") >> lit("=") >> lit("\"0.7.0.0\""))
		    >> lit(">")
		    >> *version
		    //>> *(!lit("</niftoolsxml>") >> char_) // temporary skip rule
		    >> lit("</niftoolsxml>")
		    >> eoi;

		comment = *lexeme[+(!char_('<') >> graph)];

		version =
		    lit("<version")[std::cout << val("VERSION=")]
		    >> num_attribute[std::cout << _1 << std::endl]
		    >> char_('>')
		    >> comment
		    >> lit("</version>");

		num_attribute =
		    lit("num")[_val = 0] >> '='
		    >> '"' >> (uint_[_val = _val * 256 + _1] % '.') >> '"';

		start.name("<niftoolsxml>...</niftoolsxml>");
		comment.name("...");
		version.name("<version>...</version>");
		num_attribute.name("num=\"...\"");

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
 * Main entry point for parsing an xml file.
 */

FileFormat::FileFormat(const std::string & filename)
{
	// open file, disable skipping of whitespace
	std::ifstream in(filename.c_str());
	if (in.fail()) {
		throw io_error("Could not open '" + filename + "'.");
	}
	in.unsetf(std::ios::skipws);
	std::cout << "PARSING " << filename << std::endl;

	// wrap istream into iterator
	boost::spirit::istream_iterator first(in);
	boost::spirit::istream_iterator last;

	// create skip parser
	xml_skipper<boost::spirit::istream_iterator> skipper;

	// create parser
	xml_parser<boost::spirit::istream_iterator> parser;

	// use iterator to parse file data
	bool r = qi::phrase_parse(first, last, parser, skipper);

	// fail if we did not get a full match
	if (!r || first != last)
		throw syntax_error("Syntax error while parsing '" + filename + "'.");
};

}; // namespace xml

}; // namespace object_models

}; // namespace pyffi
