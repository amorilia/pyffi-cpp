/* 
  Tree grammar which optimizes an existing FFI tree.
*/
tree grammar FFITreeOpt;

options {
    output=AST;
    tokenVocab=FFI;
    ASTLabelType=CommonTree;
    backtrack=true;
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

// notes:
// merges five or more duplicate ifs
// to merge more, simply run the optimizer multiple times (check the parser changed attribute!)

// todo:
// combine x <= y and y <= z into x <= y <= z
// combine x <= y and y <= x into x == y

@members {
    TreeWizard wiz = new TreeWizard(adaptor);
    boolean changed = false;
}

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

// XXX is there a more efficient way to do this?
class_fielddefines_ifelifelse_fragment
    :   ^(IF e1=expression d1=class_fielddefines)
        ^(IF e2=expression d2=class_fielddefines)
        ^(IF e3=expression d3=class_fielddefines)
        ^(IF e4=expression d4=class_fielddefines)
        { changed |= wiz.equals($e1.tree, $e2.tree) ||  wiz.equals($e2.tree, $e3.tree) ||  wiz.equals($e3.tree, $e4.tree); }
        -> { wiz.equals($e1.tree, $e2.tree) &&  wiz.equals($e2.tree, $e3.tree) &&  wiz.equals($e3.tree, $e4.tree) }?
        ^(IF $e1 $d1 $d2 $d3 $d4)
        -> { wiz.equals($e1.tree, $e2.tree) &&  wiz.equals($e2.tree, $e3.tree) }?
        ^(IF $e1 $d1 $d2 $d3)
        ^(IF $e3 $d4)
        -> { wiz.equals($e2.tree, $e3.tree) &&  wiz.equals($e3.tree, $e4.tree) }?
        ^(IF $e1 $d1)
        ^(IF $e3 $d2 $d3 $d4)
        -> { wiz.equals($e1.tree, $e2.tree) &&  wiz.equals($e3.tree, $e4.tree) }?
        ^(IF $e1 $d1 $d2)
        ^(IF $e3 $d3 $d4)
        -> { wiz.equals($e1.tree, $e2.tree) }?
        ^(IF $e1 $d1 $d2)
        ^(IF $e3 $d3)
        ^(IF $e4 $d4)
        -> { wiz.equals($e2.tree, $e3.tree) }?
        ^(IF $e1 $d1)
        ^(IF $e2 $d2 $d3)
        ^(IF $e4 $d4)
        -> { wiz.equals($e3.tree, $e4.tree) }?
        ^(IF $e1 $d1)
        ^(IF $e2 $d2)
        ^(IF $e3 $d3 $d4)
        ->
        ^(IF $e1 $d1)
        ^(IF $e2 $d2)
        ^(IF $e3 $d3)
        ^(IF $e4 $d4)
    |   ^(IF e1=expression d1=class_fielddefines)
        ^(IF e2=expression d2=class_fielddefines)
        ^(IF e3=expression d3=class_fielddefines)
        { changed |= wiz.equals($e1.tree, $e2.tree) ||  wiz.equals($e2.tree, $e3.tree); }
        -> { wiz.equals($e1.tree, $e2.tree) &&  wiz.equals($e2.tree, $e3.tree) }?
        ^(IF $e1 $d1 $d2 $d3)
        -> { wiz.equals($e1.tree, $e2.tree) }?
        ^(IF $e1 $d1 $d2)
        ^(IF $e3 $d3)
        -> { wiz.equals($e2.tree, $e3.tree) }?
        ^(IF $e1 $d1)
        ^(IF $e3 $d2 $d3)
        ->
        ^(IF $e1 $d1)
        ^(IF $e2 $d2)
        ^(IF $e3 $d3)
    |   ^(IF e1=expression d1=class_fielddefines)
        ^(IF e2=expression d2=class_fielddefines)
        { changed |= wiz.equals($e1.tree, $e2.tree); }
        -> { wiz.equals($e1.tree, $e2.tree) }?
        ^(IF $e1 $d1 $d2)
        ->
        ^(IF $e1 $d1)
        ^(IF $e2 $d2)
    |   ^(IF expression class_fielddefines
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
