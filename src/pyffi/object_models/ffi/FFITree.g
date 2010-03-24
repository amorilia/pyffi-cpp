/* 
  Basic tree grammar for FFI.
  This grammar is the reference for all other FFI tree grammars.
*/
tree grammar FFITree;

options {
    language=C;
    output=AST;
    ASTLabelType=pANTLR3_BASE_TREE;
    tokenVocab=FFI;
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

ffi
    :   formatdefine declarations?
    ;

doc
    :   ^(DOC SHORTDOC*)
    ;

formatdefine
    :   ^(FILEFORMAT doc CONSTANTNAME*)
    ;

declarations
    :   (typedefine | fielddefine | classdefine | enumdefine)+
    ;

enumdefine
    :   ^(ENUMDEF doc TYPENAME TYPENAME enumconstant+)
    ;

enumconstant
    :   ^(ENUMCONSTDEF doc CONSTANTNAME expression)
    ;

classdefine
    :   ^(CLASSDEF doc TYPENAME ^(BASE TYPENAME?) declarations?)
    ;

typedefine
    :   ^(TYPEDEF doc TYPENAME TYPENAME?)
    ;

fielddefine
    :   ^(FIELDDEF doc TYPENAME VARIABLENAME arguments?)
    |   ^(IF expression fielddefine+
            (^(ELIF expression fielddefine+))*
            (^(ELSE fielddefine+))?
        )
    ;

kwarg
    :   ^(KWARG VARIABLENAME expression)
    ;

arguments
    :   ^(OP_CALL kwarg+)
    ;

expression
    :   VARIABLENAME
    |   INT
    |   FLOAT
    |   STRING
    |   ^(OP_LOGICAL_OR expression expression)
    |   ^(OP_LOGICAL_AND expression expression)
    |   ^(OP_LOGICAL_NOT expression)
    |   ^(OP_EQ expression expression)
    |   ^(OP_NEQ expression expression)
    |   ^(OP_GT expression expression)
    |   ^(OP_LT expression expression)
    |   ^(OP_GTEQ expression expression)
    |   ^(OP_LTEQ expression expression)
    |   ^(OP_BITWISE_OR expression expression)
    |   ^(OP_BITWISE_AND expression expression)
    |   ^(OP_LEFTSHIFT expression expression)
    |   ^(OP_RIGHTSHIFT expression expression)
    |   ^(OP_PLUS expression expression)
    |   ^(OP_MINUS expression expression)
    |   ^(OP_NEGATE expression)
    |   ^(OP_BITWISE_NOT expression)
    |   ^(OP_MULTIPLY expression expression)
    |   ^(OP_DIVIDE expression expression)
    |   ^(OP_MODULO expression expression)
    |   ^(OP_POWER expression expression)
    ;
