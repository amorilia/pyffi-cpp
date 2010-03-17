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

ffi
    :   SHORTDOC*
        (HEADER_XML SHORTDOC*)? // not really required, but usually present
        (HEADER_DOCTYPE SHORTDOC*)? // not really required, but usually present
        t=TAG_START_ROOT anyattribute* TAG_CLOSE SHORTDOC*
        declarations
        TAG_END_ROOT SHORTDOC*
        EOF
        -> 
/*
# The nif file format.
fileformat NIF, KF, KFA
*/
           ^(FILEFORMAT
               ^(DOC SHORTDOC[$t, "The nif file format."])
               CONSTANTNAME[$t, "NIF"]
               CONSTANTNAME[$t, "KF"]
               CONSTANTNAME[$t, "KFA"]
           )
/*
class FileVersion:
    String game
    Int64 version
    Int64 user1
    Int64 user2
*/
           ^(CLASS
               DOC
               TYPENAME[$t, "FileVersion"]
               ^(FIELDDEF DOC TYPENAME[$t, "String"] VARIABLENAME[$t, "game"])
               ^(FIELDDEF DOC TYPENAME[$t, "Int64"] VARIABLENAME[$t, "version"])
               ^(FIELDDEF DOC TYPENAME[$t, "Int64"] VARIABLENAME[$t, "user1"])
               ^(FIELDDEF DOC TYPENAME[$t, "Int64"] VARIABLENAME[$t, "user2"])
           )
           declarations
    ;

// extra attributes that we throw away
// (OP_MULTIPLY handles niflibtype="*" for the Ptr type in nif.xml)
anyattribute
    :   NAME ATTR_EQ ATTR_VALUE_START (expression | OP_MULTIPLY) ATTR_VALUE_END
    ;

declarations
    :   (versiondefine | basicdefine | enumdefine | structdefine)*
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

enumdefine
    :   TAG_START_ENUM enum_nameattribute enum_storageattribute TAG_CLOSE
        doc=SHORTDOC?
        enum_option+
        TAG_END_ENUM SHORTDOC*
        -> ^(ENUMDEF ^(DOC $doc) enum_nameattribute enum_storageattribute enum_option+);

enum_nameattribute
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t];

enum_storageattribute
    :   NAME_STORAGE ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t];

enum_option
    :   TAG_START_OPTION option_valueattribute option_nameattribute TAG_CLOSE
        doc=SHORTDOC?
        TAG_END_OPTION SHORTDOC*
        -> ^(ENUMCONSTDEF ^(DOC $doc) option_nameattribute option_valueattribute);

option_valueattribute
    :   NAME_VALUE ATTR_EQ ATTR_VALUE_START INT ATTR_VALUE_END
        -> INT;

option_nameattribute
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> CONSTANTNAME[$t];

structdefine
    :   TAG_START_STRUCT struct_nameattribute anyattribute* TAG_CLOSE
    	doc=SHORTDOC?
        struct_add*
        TAG_END_STRUCT SHORTDOC*
        -> ^(CLASSDEF ^(DOC $doc) struct_nameattribute struct_add*);

struct_nameattribute
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, $t.text];

struct_add
    :   TAG_START_ADD add_nameattribute add_typeattribute anyattribute* TAG_CLOSE
        doc=SHORTDOC?
        TAG_END_ADD SHORTDOC*
        -> ^(FIELDDEF ^(DOC $doc) add_typeattribute add_nameattribute);

add_nameattribute
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> VARIABLENAME[$t, $t.text];

add_typeattribute
    :   NAME_TYPE ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, $t.text];

expression
	:   or_test
    ;

or_test
	:   and_test (OP_LOGICAL_OR and_test)*
    ;

and_test
    :   not_test (OP_LOGICAL_AND not_test)*
    ;

not_test
	:   OP_NOT not_test
	|   comparison
	;

comparison
    :   or_expr (comp_op or_expr)*
    ;

comp_op
	:   OP_EQ
	|   OP_NEQ
	|   OP_GT
	|   OP_LT
	|   OP_GTEQ
	|   OP_LTEQ
	;

or_expr
    : and_expr (OP_BITWISE_OR and_expr)*
	;

and_expr
    : shift_expr (OP_BITWISE_AND shift_expr)*
	;

shift_expr
    : arith_expr ((OP_LEFTSHIFT|OP_RIGHTSHIFT) arith_expr)*
	;

arith_expr
    : term ((OP_PLUS|OP_MINUS) term)*
	;

term
    : atom ((OP_MULTIPLY | OP_DIVIDE | OP_MODULO) atom)*
	;

atom
    :   NAME
    |   INT
    |   FLOAT
    |   LBRACKET expression RBRACKET
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
    :   { tagMode && attrMode }?=> '0x' ( '0' .. '9' | 'a' .. 'f' | 'A' .. 'F' )+
    // octal
    |   { tagMode && attrMode }?=> '0o' ( '0' .. '7')+
    // binary
    |   { tagMode && attrMode }?=> '0b' ( '0' .. '1')+
    // decimal, or version integer
    |   { tagMode && attrMode }?=> DIGITS ('.' DIGITS)*
    ;

fragment
EXPONENT
    :    'e' ( '+' | '-' )? DIGITS
    ;

FLOAT
    :   { tagMode && attrMode }?=> DIGITS '.' DIGITS EXPONENT? 'f'
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

TAG_START_OPTION
    :   { !tagMode }?=> '<option' { tagMode = true; }
    ;

TAG_END_OPTION
    :   { !tagMode }?=> '</option>'
    ;

TAG_START_STRUCT
    :   { !tagMode }?=> ('<' ('compound'|'niobject'|'struct')) { tagMode = true; }
    ;

TAG_END_STRUCT
    :   { !tagMode }?=> ('</' ('compound'|'niobject'|'struct') '>')
    ;

TAG_START_ADD
    :   { !tagMode }?=> '<add' { tagMode = true; }
    ;

TAG_END_ADD
    :   { !tagMode }?=> '</add>'
    ;

TAG_CLOSE
    :   { tagMode && !attrMode }?=> '>' { tagMode = false; }
    ;

TAG_END
    :   { tagMode && !attrMode }?=> '/>' { tagMode = false; }
    ;

ATTR_EQ
    :   { tagMode && !attrMode }?=> '='
    ;

ATTR_VALUE_START
    :   { tagMode && !attrMode }?=> '"' { attrMode = true; }
    ;

ATTR_VALUE_END
    :   { tagMode && attrMode }?=> { attrMode = false; } '"'
    ;

NAME_NUM
    :   { tagMode && !attrMode }?=> 'num'
    ;

NAME_NAME
    :   { tagMode && !attrMode }?=> 'name'
    ;

NAME_VALUE
    :   { tagMode && !attrMode }?=> 'value'
    ;

NAME_STORAGE
    :   { tagMode && !attrMode }?=> 'storage'
    ;

NAME_TYPE
    :   { tagMode && !attrMode }?=> 'type'
    ;

NAME
    :   { tagMode && !attrMode }?=> (UCLETTER | LCLETTER) (UCLETTER | LCLETTER | DIGIT)*
    |   { tagMode && attrMode }?=> (UCLETTER | LCLETTER) (UCLETTER | LCLETTER | DIGIT | '_' | ' ')*
    ;

OP_EQ
    :   { tagMode && attrMode }?=> '==';

OP_NEQ
    :   { tagMode && attrMode }?=> '!=';

OP_RIGHTSHIFT
    :   { tagMode && attrMode }?=> '>>';

OP_LEFTSHIFT
    :   { tagMode && attrMode }?=> '<<';

OP_GTEQ
    :   { tagMode && attrMode }?=> '>=';

OP_LTEQ
    :   { tagMode && attrMode }?=> '<=';

OP_GT
    :   { tagMode && attrMode }?=> '>';

OP_LT
    :   { tagMode && attrMode }?=> '<';

OP_NOT
    :   { tagMode && attrMode }?=> '!';

OP_LOGICAL_AND
    :   { tagMode && attrMode }?=> '&amp;&amp;';

OP_BITWISE_AND
    :   { tagMode && attrMode }?=> '&amp;';

OP_LOGICAL_OR
    :   { tagMode && attrMode }?=> '||';

OP_BITWISE_OR
    :   { tagMode && attrMode }?=> '|';

OP_MINUS
    :   { tagMode && attrMode }?=> '-';

OP_PLUS
    :   { tagMode && attrMode }?=> '-';

OP_MULTIPLY
    :   { tagMode && attrMode }?=> '*';

OP_DIVIDE
    :   { tagMode && attrMode }?=> '/';

OP_MODULO
    :   { tagMode && attrMode }?=> '%';

LBRACKET
    :   { tagMode && attrMode }?=> '(';

RBRACKET
    :   { tagMode && attrMode }?=> ')';

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
