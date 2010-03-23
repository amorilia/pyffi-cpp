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
    :   (typedefine | parameterdefine | classdefine | enumdefine)+
    ;

enumdefine
    :   ^(ENUMDEF doc TYPENAME TYPENAME enumconstant+)
    ;

enumconstant
    :   ^(ENUMCONSTDEF doc CONSTANTNAME expression)
    ;

classdefine
    :   ^(CLASSDEF doc TYPENAME ^(BASE TYPENAME?) declarations? class_fielddefines?)
    ;

typedefine
    :   ^(TYPEDEF doc TYPENAME TYPENAME?)
    ;

parameterdefine
    :   ^(PARAMETERDEF doc TYPENAME VARIABLENAME fieldparameters?)
    ;

fielddefine
    :   ^(FIELDDEF doc TYPENAME VARIABLENAME fieldparameters?)
    ;

class_fielddefines_ifelifelse_fragment
    :   ^(IF expression class_fielddefines
            (^(ELIF expression class_fielddefines))*
            (^(ELSE class_fielddefines))?
        )
    ;

class_fielddefines
    :   class_fielddefine+
    ;

class_fielddefine
    :   class_fielddefines_ifelifelse_fragment
    |   fielddefine
    ;

kwarg
    :   ^(FIELDARG VARIABLENAME expression)
    ;

fieldparameters
    :   ^(FIELDARGLIST kwarg+)
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
