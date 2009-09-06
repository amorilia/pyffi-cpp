grammar XML;

options {
    output=AST;
    tokenVocab=FFI;
    language=C;
    ASTLabelType=pANTLR3_BASE_TREE;
}

@lexer::members {
    bool tagMode = false;
    bool attrMode = false;
        }

@parser::header {
    #include <string>
}

@parser::members {
    // converts varname into a string formatted as a variable name
    // (lower_case_with_underscores)
    std::string newVarString(pANTLR3_STRING varname) {
        // new length never exceeds original length
        std::string buf;
        int i = 0; // index into varname
        bool underscore = false; // if last character added was an underscore
        while ((varname->chars[i] == ' ') && (i < varname->len)) i++;
        while (i < varname->len) {
            if ((varname->chars[i] >= 'A') && (varname->chars[i] <= 'Z')) {
                buf.push_back(varname->chars[i++] - 'A' + 'a');
                underscore = false;
            } else if ((varname->chars[i] >= 'a') && (varname->chars[i] <= 'z')) {
                buf.push_back(varname->chars[i++]);
                underscore = false;
            } else if ((varname->chars[i] >= '0') && (varname->chars[i] <= '9')) {
                buf.push_back(varname->chars[i++]);
                underscore = false;
            } else if (!underscore) {
                // any other symbol becomes an underscore
                buf.push_back('_');
                i++;
                underscore = true;
            } else {
                // do not put more than two underscores in a row
                i++;
            }
        }
        return buf;
    }
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
    :   SHORTDOC*
        (HEADER_XML SHORTDOC*)? // not really required, but usually present
        (HEADER_DOCTYPE SHORTDOC*)? // not really required, but usually present
        TAG_START_ROOT anyattribute* TAG_CLOSE SHORTDOC*
        declarations
        TAG_END_ROOT SHORTDOC*
        EOF
        -> ^(FILEFORMAT DOC) declarations
    ;

// extra attributes that we throw away
anyattribute
    :   NAME ATTR_EQ ATTR_VALUE_START .* ATTR_VALUE_END
    ;

declarations
    :   (versiondefine | basicdefine /* | enumdefine | classdefine */ )*
/*
class FileVersion:
    String game
    Int64 version
    Int64 user1
    Int64 user2
    Int64 user3
*/
/*
           ^(CLASS
               DOC
               TYPENAME
               ^(FIELDDEF DOC NAME_STRING NAME_GAME)
               ^(FIELDDEF DOC NAME_INT64 NAME_VERSION)
               ^(FIELDDEF DOC NAME_INT64 NAME_USERVERSION)
               ^(FIELDDEF DOC NAME_INT64 NAME_USERVERSION2)
               ^(FIELDDEF DOC NAME_INT64 NAME_USERVERSION3)
           )
*/
    ;

versiondefine
    :   TAG_START_VERSION
        NAME_NUM ATTR_EQ ATTR_VALUE_START v=INT ATTR_VALUE_END
        TAG_CLOSE
        doc=SHORTDOC
        TAG_END_VERSION SHORTDOC*
/*
<version num="1.2.3">Game Name</version>

->

parameter:
    FileVersion ver_1_2_3(game="Game Name", version="1.2.3")
*/
        -> ^(PARAMETER ^(FIELDDEF DOC TYPENAME[$v, "FileVersion"] VARIABLENAME[$v, (std::string("ver_") + newVarString($v.text)).c_str()]
               ^(FIELDARGLIST
                  ^(FIELDARG VARIABLENAME[$doc, "game"] STRING[$doc, ($doc.text)->chars])
                  ^(FIELDARG VARIABLENAME[$v, "version"] INT)
               )
           ))
    ;

// matches typedefine in FFIFileFormat
basicdefine
    :   TAG_START_BASIC basic_nameattribute anyattribute* TAG_CLOSE
        doc=SHORTDOC?
        TAG_END_BASIC SHORTDOC*
        -> ^(TYPEDEF ^(DOC $doc?) basic_nameattribute)
    ;

basic_nameattribute
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, ($t.text)->chars]
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

TAG_START_BASIC
    :   { !tagMode }?=> '<basic' { tagMode = true; }
    ;

TAG_END_BASIC
    :   { !tagMode }?=> '</basic>'
    ;

TAG_START_ENUM
    :   { !tagMode }?=> '<enum' { tagMode = true; }
    ;

TAG_END_ENUM
    :   { !tagMode }?=> '</enum>'
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

NAME_NAME
    :   { tagMode }?=> 'name'
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
SHORTDOC
    :   { !tagMode }?=> (~'<')*
    ;

// temporary rule to match anything not yet implemented    
ANY
    : .
    ;
