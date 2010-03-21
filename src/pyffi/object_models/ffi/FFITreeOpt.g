/* 
  Tree grammar which optimizes an existing FFI tree.
*/
tree grammar FFITreeOpt;

options {
    output=AST;
    tokenVocab=FFI;
    ASTLabelType=CommonTree;
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

// XXX at the moment, does nothing
// TODO merge duplicate ifs
//      combine x <= y and y <= z into x <= y <= z
//      combine x <= y and y <= x into x == y

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
    :   ^(CLASSDEF doc TYPENAME ^(BASE TYPENAME) declarations? class_fielddefines?)
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
    :   (class_fielddefines_ifelifelse_fragment | fielddefine)+
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
    |   ^(OP_LOGICAL_OR e1=expression e2=expression)
    |   ^(OP_LOGICAL_AND e1=expression e2=expression)
    |   ^(OP_LOGICAL_NOT expression)
    |   ^(OP_EQ e1=expression e2=expression)
    |   ^(OP_NEQ e1=expression e2=expression)
    |   ^(OP_GT e1=expression e2=expression)
    |   ^(OP_LT e1=expression e2=expression)
    |   ^(OP_GTEQ e1=expression e2=expression)
    |   ^(OP_LTEQ e1=expression e2=expression)
    |   ^(OP_BITWISE_OR e1=expression e2=expression)
    |   ^(OP_BITWISE_AND e1=expression e2=expression)
    |   ^(OP_LEFTSHIFT e1=expression e2=expression)
    |   ^(OP_RIGHTSHIFT e1=expression e2=expression)
    |   ^(OP_PLUS e1=expression e2=expression)
    |   ^(OP_MINUS e1=expression e2=expression)
    |   ^(OP_NEGATE e1=expression)
    |   ^(OP_BITWISE_NOT e1=expression)
    |   ^(OP_MULTIPLY e1=expression e2=expression)
    |   ^(OP_DIVIDE e1=expression e2=expression)
    |   ^(OP_MODULO e1=expression e2=expression)
    |   ^(OP_POWER e1=expression e2=expression)
    ;
