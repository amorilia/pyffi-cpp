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
    // doesn't work in header, so include it here
    #include <boost/algorithm/string.hpp>

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
        // strip trailing underscores (from trailing blanks)
        boost::trim_right_if(buf, boost::is_any_of("_"));
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
        declarations?
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
/* XXX deactivated for now
           ^(CLASSDEF
               ^(DOC SHORTDOC[$t, "Describes the file version."])
               TYPENAME[$t, "FileVersion"]
               ^(BASE)
               ^(FIELDDEF DOC TYPENAME[$t, "String"] VARIABLENAME[$t, "game"])
               ^(FIELDDEF DOC TYPENAME[$t, "Int64"] VARIABLENAME[$t, "version"])
               ^(FIELDDEF DOC TYPENAME[$t, "Int64"] VARIABLENAME[$t, "user1"])
               ^(FIELDDEF DOC TYPENAME[$t, "Int64"] VARIABLENAME[$t, "user2"])
           )
*/
           declarations?
    ;

// extra attributes that we throw away
// (OP_MULTIPLY handles niflibtype="*" for the Ptr type in nif.xml)
anyattribute
    :   NAME ATTR_EQ ATTR_VALUE_START (expression | OP_MULTIPLY) ATTR_VALUE_END
    ;

// XXX temporarily ignore versiondefine
declarations
    :   (versiondefine! | basicdefine | enumdefine | structdefine | bitflagsdefine)+
    ;

versiondefine
    :   TAG_START_VERSION
        NAME_NUM ATTR_EQ ATTR_VALUE_START v=INT ATTR_VALUE_END
        TAG_CLOSE
        (doc=SHORTDOC)*
        TAG_END_VERSION SHORTDOC*
/*
<version num="1.2.3">Game Name</version>

->

static FileVersion ver_1_2_3(game="Game Name", version="1.2.3")
*/
        -> ^(FIELDDEF DOC TYPENAME[$v, "FileVersion"] VARIABLENAME[$v, (std::string("ver_") + newVarString($v.text)).c_str()]
               ^(OP_CALL
                  ^(KWARG VARIABLENAME[$doc, "game"] STRING[$doc, ($doc.text)->chars])
                  ^(KWARG VARIABLENAME[$v, "version"] INT)
               )
        )
    ;

basicdefine
    :   TAG_START_BASIC (name=attr_type_name|anyattribute)* TAG_CLOSE
        (doc+=SHORTDOC)*
        TAG_END_BASIC SHORTDOC*
        -> ^(CLASSDEF ^(DOC $doc*) $name BASE)
    ;

enumdefine
    :   TAG_START_ENUM
        ( name=attr_type_name
        | type=attr_type_storage
        | attr_expression_ver1
        | attr_expression_ver2
        | anyattribute
        )* TAG_CLOSE
        (doc+=SHORTDOC)*
        enum_option+
        TAG_END_ENUM SHORTDOC*
        -> ^(ENUMDEF ^(DOC $doc*) $name $type enum_option+)
    ;

enum_option
    :   TAG_START_OPTION
        ( value=attr_expression_value
        | name=attr_constant_name 
        | attr_expression_default
        | anyattribute
        )*
        ( TAG_CLOSE (doc+=SHORTDOC)* TAG_END_OPTION
        | TAG_END
        )
        SHORTDOC*
        -> ^(ENUMCONSTDEF ^(DOC $doc*) $name $value)
    ;

structdefine
    :   TAG_START_STRUCT
        ( name=attr_type_name
        | base=attr_type_inherit
        | attr_expression_ver1
        | attr_expression_ver2
        | anyattribute
        )* TAG_CLOSE
    	(doc+=SHORTDOC)*
        struct_add*
        TAG_END_STRUCT SHORTDOC*
        -> ^(CLASSDEF ^(DOC $doc*) $name ^(BASE $base?) struct_add*)
    ;

// for the time being, bitflags are converted to enums
// TODO: implement bitstructs in the FFI grammar and use them here
bitflagsdefine
    :   TAG_START_BITFLAGS
        ( name=attr_type_name
        | type=attr_type_storage
        )* TAG_CLOSE
    	(doc+=SHORTDOC)*
        enum_option+
        TAG_END_BITFLAGS SHORTDOC*
        -> ^(ENUMDEF ^(DOC $doc*) $name $type enum_option+)
    ;

// note: for simplicity of implementation, struct_add always emits an
//       OP_INDEX node, even if there are no arrays the optimizer
//       removes these nodes later
struct_add
@init {
    bool has_ver1 = false;
    bool has_ver2 = false;
    bool has_cond = false;
    bool has_vercond = false;
}
    :   TAG_START_ADD
        ( name=attr_variable_name
        | type=attr_type_type
        | {has_ver1=true;} ver1=attr_expression_ver1
        | {has_ver2=true;} ver2=attr_expression_ver2
        | {has_cond=true;} cond=attr_expression_cond
        | {has_vercond=true;} vercond=attr_expression_vercond
        | arr1=attr_expression_arr1
        | arr2=attr_expression_arr2
        | attr_expression_default
        | anyattribute
        )*
        ( TAG_CLOSE
          (doc+=SHORTDOC)*
          TAG_END_ADD
        | TAG_END
        )
        SHORTDOC*
        // has_cond && has_vercond
        -> {has_ver1 && has_ver2 && has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                    ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                        ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                            $ver1 $ver2)
                        $cond)
                    $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {has_ver1 && !has_ver2 && has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                    ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                        $ver1 $cond)
                    $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && has_ver2 && has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                    ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                        $ver2 $cond)
                    $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && !has_ver2 && has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"] $cond $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        // !has_cond && has_vercond
        -> {has_ver1 && has_ver2 && !has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                    ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                        $ver1 $ver2)
                    $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {has_ver1 && !has_ver2 && !has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"] $ver1 $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && has_ver2 && !has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"] $ver2 $vercond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && !has_ver2 && !has_cond && has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                $vercond
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        // has_cond && !has_vercond
        -> {has_ver1 && has_ver2 && has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                    ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"]
                        $ver1 $ver2)
                    $cond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {has_ver1 && !has_ver2 && has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"] $ver1 $cond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && has_ver2 && has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"] $ver2 $cond)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && !has_ver2 && has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                $cond
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        // !has_cond && !has_vercond
        -> {has_ver1 && has_ver2 && !has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                ^(OP_LOGICAL_AND[$TAG_START_ADD, "and"] $ver1 $ver2)
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {has_ver1 && !has_ver2 && !has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                $ver1
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> {!has_ver1 && has_ver2 && !has_cond && !has_vercond}?
           ^(IF[$TAG_START_ADD, "if"]
                $ver2
                ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?)))
        -> ^(FIELDDEF ^(DOC $doc*) $type $name ^(OP_INDEX $arr1? $arr2?))
    ;

attr_variable_name
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> VARIABLENAME[$t, newVarString($t.text).c_str()]
    ;

attr_type_inherit
    :   NAME_INHERIT ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, ($t.text)->chars]
    ;

attr_type_name
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, ($t.text)->chars]
    ;

attr_type_storage
    :   NAME_STORAGE ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, ($t.text)->chars]
    ;

attr_type_type
    :   NAME_TYPE ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> TYPENAME[$t, ($t.text)->chars]
    ;

attr_constant_name
    :   NAME_NAME ATTR_EQ ATTR_VALUE_START t=NAME ATTR_VALUE_END
        -> CONSTANTNAME[$t, ($t.text)->chars]
    ;

attr_expression_value
    :   NAME_VALUE! ATTR_EQ! ATTR_VALUE_START! expression ATTR_VALUE_END!
    ;

attr_expression_ver1
    :   NAME_VER1 ATTR_EQ ATTR_VALUE_START v=INT ATTR_VALUE_END
        -> ^(OP_GTEQ[$NAME_VER1, ">="] VARIABLENAME[$NAME_VER1, "version"] $v)
    ;

attr_expression_ver2
    :   NAME_VER2 ATTR_EQ ATTR_VALUE_START v=INT ATTR_VALUE_END
        -> ^(OP_LTEQ[$NAME_VER2, "<="] VARIABLENAME[$NAME_VER2, "version"] $v)
    ;

attr_expression_cond
    :   NAME_COND! ATTR_EQ! ATTR_VALUE_START! expression ATTR_VALUE_END!
    ;

attr_expression_vercond
    :   NAME_VERCOND! ATTR_EQ! ATTR_VALUE_START! expression ATTR_VALUE_END!
    ;

attr_expression_default
    :   NAME_DEFAULT ATTR_EQ ATTR_VALUE_START .* ATTR_VALUE_END
    ;

attr_expression_arr1
    :   NAME_ARR1! ATTR_EQ! ATTR_VALUE_START! expression ATTR_VALUE_END!
    ;

attr_expression_arr2
    :   NAME_ARR2! ATTR_EQ! ATTR_VALUE_START! expression ATTR_VALUE_END!
    ;

expression
	:   or_test
    ;

or_test
	:   and_test (OP_LOGICAL_OR^ and_test)*
    ;

and_test
    :   not_test (OP_LOGICAL_AND^ not_test)*
    ;

not_test
	:   OP_LOGICAL_NOT^ not_test
	|   comparison
	;

comparison
    :   or_expr (comp_op^ or_expr)*
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
    :   and_expr (OP_BITWISE_OR^ and_expr)*
	;

and_expr
    :   shift_expr (OP_BITWISE_AND^ shift_expr)*
	;

shift_expr
    :   arith_expr ((OP_LEFTSHIFT^|OP_RIGHTSHIFT^) arith_expr)*
	;

arith_expr
    :   term ((OP_PLUS^|OP_MINUS^) term)*
	;

term
    :   factor ((OP_MULTIPLY^ | OP_DIVIDE^ | OP_MODULO^) factor)*
	;

factor
    :   OP_PLUS! factor
    |   op=OP_MINUS factor
        -> ^(OP_NEGATE[$op, "-"] factor)
    |   OP_BITWISE_NOT^ factor
	|   power
	;

power
	:   atom (options {greedy=true;}:OP_POWER^ factor)?
	;

atom
    :   NAME
        -> VARIABLENAME[$NAME, newVarString($NAME.text).c_str()]
    |   INT
    |   FLOAT
    |   LBRACKET! expression RBRACKET!
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

TAG_START_BITFLAGS
    :   { !tagMode }?=> '<bitflags' { tagMode = true; }
    ;

TAG_END_BITFLAGS
    :   { !tagMode }?=> '</bitflags>'
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

NAME_INHERIT
    :   { tagMode && !attrMode }?=> 'inherit'
    ;

NAME_TYPE
    :   { tagMode && !attrMode }?=> 'type'
    ;

NAME_VER1
    :   { tagMode && !attrMode }?=> 'ver1'
    ;

NAME_VER2
    :   { tagMode && !attrMode }?=> 'ver2'
    ;

NAME_COND
    :   { tagMode && !attrMode }?=> 'cond'
    ;

NAME_VERCOND
    :   { tagMode && !attrMode }?=> 'vercond'
    ;

NAME_DEFAULT
    :   { tagMode && !attrMode }?=> 'default'
    ;

NAME_ARR1
    :   { tagMode && !attrMode }?=> 'arr1'
    ;

NAME_ARR2
    :   { tagMode && !attrMode }?=> 'arr2'
    ;

NAME
    :   { tagMode && !attrMode }?=> (UCLETTER | LCLETTER) (UCLETTER | LCLETTER | DIGIT)*
    |   { tagMode && attrMode }?=> (UCLETTER | LCLETTER) (UCLETTER | LCLETTER | DIGIT | '_' | ' ' | '?')*
    ;

OP_EQ
    :   { tagMode && attrMode }?=> '==';

OP_NEQ
    :   { tagMode && attrMode }?=> '!=';

OP_RIGHTSHIFT
    :   { tagMode && attrMode }?=> ('>>' | '&gt;&gt;');

OP_LEFTSHIFT
    :   { tagMode && attrMode }?=> ('<<' | '&lt;&lt;');

OP_GTEQ
    :   { tagMode && attrMode }?=> ('>=' | '&gt;=');

OP_LTEQ
    :   { tagMode && attrMode }?=> ('<=' | '&lt;=');

OP_GT
    :   { tagMode && attrMode }?=> ('>' | '&gt;');

OP_LT
    :   { tagMode && attrMode }?=> ('<' | '&lt;');

OP_LOGICAL_NOT
    :   { tagMode && attrMode }?=> '!';

OP_BITWISE_NOT
    :   { tagMode && attrMode }?=> '~';

OP_LOGICAL_AND
    :   { tagMode && attrMode }?=> '&amp;&amp;';

OP_BITWISE_AND
    :   { tagMode && attrMode }?=> '&amp;';

OP_LOGICAL_OR
    :   { tagMode && attrMode }?=> '||';

OP_BITWISE_OR
    :   { tagMode && attrMode }?=> '|';

OP_POWER
    :   { tagMode && attrMode }?=> '**';

OP_MINUS
    :   { tagMode && attrMode }?=> '-';

OP_PLUS
    :   { tagMode && attrMode }?=> '+';

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
// and leading outside tagmode
WS
    :   { tagMode }?=> (' ' | '\t' | '\r' | '\n' )+ { $channel=HIDDEN; }
    |   { !tagMode }?=> (' '|'\t')* '\r'? '\n' (' '|'\t')* { $channel=HIDDEN; }
    ;



// any text until the next tag
SHORTDOC
    :   { !tagMode }?=> (~('<'|'\n'))*
    ;

// temporary rule to match anything not yet implemented    
ANY
    : .
    ;
