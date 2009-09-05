grammar XML;

options {
    output=AST;
    language=C;
    ASTLabelType=pANTLR3_BASE_TREE;
    tokenVocab=FFI;
}

@lexer::members {
    bool tagMode = false;
    bool attrMode = false;
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

/*

Note about whitespace:

We ignore whitespace in tags by sending it to the hidden channel
but not in between tags because it might be part of the documentation.
However the lexer does not know about the structure of the file, and
cannot make the distinction between text that can be ignored, and text
that is part of the documentation. So, we delegate this distinction to the
parser:

  * whitespace or text in between tags is explicitly modelled through
    "anytext", which should never be part of the AST; however

  * if the text is documentation, use "doctext", and add it to the AST.

*/

ffi
    :   anytext
        (HEADER_XML anytext)? // not really required, but usually present
        (HEADER_DOCTYPE anytext)? // not really required, but usually present
	TAG_START_ROOT anyattributes TAG_CLOSE anytext
	declarations
	TAG_END_ROOT anytext
	EOF
        -> ^(FILEFORMAT DOC) declarations
    ;

anytext
    :   TEXT*
    ;

doctext
    :   TEXT*
        -> ^(DOC TEXT*)
    ;

// extra attributes that we throw away
anyattributes
    :   (NAME ATTR_EQ ATTR_VALUE_START .* ATTR_VALUE_END)*
    ;

declarations
    :   version_block* //basicblock* enumblock* classblock*
    ;

version_block
    :   TAG_START_VERSION version_numattribute TAG_CLOSE
        doctext
        TAG_END_VERSION anytext
        -> ^(VERSION version_numattribute doctext)
    ;

version_numattribute
    :   NAME_NUM ATTR_EQ ATTR_VALUE_START INT ATTR_VALUE_END
        -> INT
    ;

/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/

// useful fragments

fragment
DIGIT
    :   '0'..'9'
    ;

fragment
DIGITS
    :   DIGIT+
    ;

INT
    // hex
    :   '0x' ( '0' .. '9' | 'a' .. 'f' | 'A' .. 'F' )+
    // octal
    |   '0o' ( '0' .. '7')+
    // binary
    |   '0b' ( '0' .. '1')+
    // decimal, or version integer
    |   DIGITS ('.' DIGITS)*
    ;

fragment
EXPONENT
    :    'e' ( '+' | '-' )? DIGITS
    ;

FLOAT
    :   DIGITS '.' DIGITS EXPONENT? 'f'
    ;

fragment
LCLETTER
    :   'a'..'z'
    ;

fragment
UCLETTER
    :   'A'..'Z'
    ;

TAG_START_ROOT
    :   { !tagMode }?=> '<niftoolsxml' { tagMode = true; }
    ;

TAG_END_ROOT
    :   { !tagMode }?=> '</niftoolsxml>'
    ;

TAG_START_VERSION
    :   { !tagMode }?=> '<version' { tagMode = true; }
    ;

TAG_END_VERSION
    :   { !tagMode }?=> '</version>'
    ;

TAG_CLOSE
    :   { tagMode }?=> '>' { tagMode = false; }
    ;

TAG_END
    :   { tagMode }?=> '/>' { tagMode = false; }
    ;

ATTR_EQ
    :   { tagMode }?=> '='
    ;

ATTR_VALUE_START
    :   { tagMode && !attrMode }?=> '"' { attrMode = true; }
    ;

ATTR_VALUE_END
    :   { tagMode && attrMode }?=> { attrMode = false; } '"'
    ;

NAME_NUM
    :   { tagMode }?=> 'num'
    ;

NAME
    :   { tagMode }?=> (UCLETTER | LCLETTER) (UCLETTER | LCLETTER | DIGIT)*
    ;

HEADER_XML
    :   { !tagMode }?=> '<?xml' .* '?>'
    ;

HEADER_DOCTYPE
    :   { !tagMode }?=> '<!DOCTYPE niftoolsxml>'
    ;

COMMENTS
    :   { !tagMode }?=> '<!--' .* '-->' { $channel=HIDDEN; }
    ;

// ignore whitespace in tagmode
WS
    :   { tagMode }?=> (' ' | '\t' | '\r' | '\n' )+ { $channel=HIDDEN; }
    ;

// any text until the next tag
TEXT
    :   { !tagMode }?=> (~'<')*
    ;

// temporary rule to match anything not yet implemented    
ANY
    : .
    ;
