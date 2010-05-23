/*

Copyright (c) 2007-2010, Python File Format Interface
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
	qi::rule<Iterator, Skipper> tag_fileformat;
	qi::rule<Iterator, Skipper> comment;
	qi::rule<Iterator, Skipper> tag_version;
	qi::rule<Iterator, Skipper> tag_basic;
	qi::rule<Iterator, Skipper> tag_enum;
	qi::rule<Iterator, Skipper> tag_option;
	qi::rule<Iterator, Skipper> tag_bitflags;
	qi::rule<Iterator, Skipper> tag_struct;
	qi::rule<Iterator, Skipper> tag_add;
	qi::rule<Iterator, std::string(std::string), Skipper> attr_string;
	qi::rule<Iterator, unsigned(std::string), Skipper> attr_version;

	xml_parser()
		: xml_parser::base_type(tag_fileformat, "fileformatxml") {
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
		using qi::_r1;
		using qi::_val;

		tag_fileformat =
		    -(lit("<?xml")
		      [std::cout << val("XML") << std::endl]
		      >> *(!lit("?>") >> char_)
		      >> lit("?>"))
		    >> -(lit("<!DOCTYPE")
		         [std::cout << val("DOCTYPE") << std::endl]
		         >> lit("niftoolsxml")
		         >> lit(">"))
		    >> lit("<niftoolsxml")
		    [std::cout << val("NIFTOOLSXML") << std::endl]
		    >> -(lit("version") >> lit("=") >> lit("\"0.7.0.0\""))
		    >> lit(">")
		    >> *(tag_version | tag_basic | tag_enum | tag_struct)
		    //>> *(!lit("</niftoolsxml>") >> char_) // temporary skip rule
		    >> lit("</niftoolsxml>")
		    >> eoi;

		comment = *lexeme[+(!char_('<') >> graph)];

		tag_add =
		    lit("<add")
		    [std::cout << val("ADD=")]
		    >> *(
		        attr_string(std::string("name"))
		        [std::cout << _1 << std::endl]
		        | attr_string(std::string("type"))
		        | attr_string(std::string("default"))
		        | attr_version(std::string("ver1"))
		        | attr_version(std::string("ver2"))
		        | attr_string(std::string("cond"))
		        | attr_string(std::string("arr1"))
		        | attr_string(std::string("arr2"))
		        | attr_string(std::string("template"))
		        | attr_string(std::string("vercond"))
		    )
		    >> char_('>')
		    >> comment
		    >> lit("</add>");

		tag_basic =
		    lit("<basic")
		    [std::cout << val("BASIC=")]
		    >> *(
		        attr_string(std::string("name"))
		        [std::cout << _1 << std::endl]
		        | attr_string(std::string("count"))
		        | attr_string(std::string("niflibtype"))
		        | attr_string(std::string("nifskopetype"))
		        | attr_string(std::string("istemplate"))
		    )
		    >> char_('>')
		    >> comment
		    >> lit("</basic>");

		tag_enum =
		    lit("<enum")
		    [std::cout << val("ENUM=")]
		    >> *(
		        attr_string(std::string("name"))
		        [std::cout << _1 << std::endl]
		        |
		        attr_string(std::string("storage"))
		        [std::cout << val("(") << _1 << ")" << std::endl]
		    )
		    >> char_('>')
		    >> comment
		    >> *tag_option
		    >> lit("</enum>");

		tag_option =
		    lit("<option")
		    [std::cout << val("OPTION=")]
		    >> *(
		        attr_string(std::string("name"))
		        [std::cout << _1 << std::endl]
		        |
		        attr_string(std::string("value"))
		    )
		    >> char_('>')
		    >> comment
		    >> lit("</option>");

		tag_struct =
		    (lit("<struct") | lit("<compound") | lit("<niobject"))
		    [std::cout << val("STRUCT=")]
		    >> *(
		        attr_string(std::string("name"))
		        [std::cout << _1 << std::endl]
		        |
		        attr_string(std::string("inherit"))
		        [std::cout << val("(") << _1 << ")" << std::endl]
		        | attr_string(std::string("niflibtype"))
		        | attr_string(std::string("nifskopetype"))
		        | attr_string(std::string("abstract"))
		        | attr_string(std::string("ver1"))
		        | attr_string(std::string("ver2"))
		    )
		    >> char_('>')
		    >> comment
		    >> *tag_add
		    >> (lit("</struct>") | lit("</compound>") | lit("</niobject>"));

		tag_version =
		    lit("<version")
		    [std::cout << val("VERSION=")]
		    >> attr_version(std::string("num"))
		    [std::cout << _1 << std::endl]
		    >> char_('>')
		    >> comment
		    >> lit("</version>");

		attr_string %=
		    lit(_r1) >> '='
		    >> lexeme['"' >> *(char_ - '"') >> '"'];

		attr_version =
		    lit(_r1)[_val = 0] >> '='
		    >> '"' >> (uint_[_val = _val * 256 + _1] % '.') >> '"';

		tag_fileformat.name("<niftoolsxml>...</niftoolsxml>");
		comment.name("...");
		tag_version.name("<version>...</version>");
		tag_basic.name("<basic>...</basic>");
		attr_string.name("attribute (string)");
		attr_version.name("attribute (version)");

		on_error<fail>(
		    tag_fileformat,
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
