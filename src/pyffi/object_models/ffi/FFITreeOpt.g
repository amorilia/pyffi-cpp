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
// only merges five or more duplicate ifs
// to merge more, simply run the optimizer multiple times (check the
// parser "again" attribute!)

// todo:
// combine x <= y and y <= z into x <= y <= z
// combine x <= y and y <= x into x == y

@members {
    TreeWizard wiz = new TreeWizard(adaptor);
    boolean again = false;
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
    :   (fielddefine | classdefine | enumdefine)+
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

fielddefine
    :   ^(FIELDDEF doc TYPENAME VARIABLENAME (OP_INDEX|indices)? arguments?)
        -> ^(FIELDDEF doc TYPENAME VARIABLENAME indices? arguments?)
    |
        // XXX is there a more efficient way to do this?
        ^(IF e1=expression d1=fielddefines)
        ^(IF e2=expression d2=fielddefines)
        ^(IF e3=expression d3=fielddefines)
        ^(IF e4=expression d4=fielddefines)
        { again |= wiz.equals($e1.tree, $e2.tree) ||  wiz.equals($e2.tree, $e3.tree) ||  wiz.equals($e3.tree, $e4.tree); }
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
    |   ^(IF e1=expression d1=fielddefines)
        ^(IF e2=expression d2=fielddefines)
        ^(IF e3=expression d3=fielddefines)
        { again |= wiz.equals($e1.tree, $e2.tree) ||  wiz.equals($e2.tree, $e3.tree); }
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
    |   ^(IF e1=expression d1=fielddefines)
        ^(IF e2=expression d2=fielddefines)
        { again |= wiz.equals($e1.tree, $e2.tree); }
        -> { wiz.equals($e1.tree, $e2.tree) }?
        ^(IF $e1 $d1 $d2)
        ->
        ^(IF $e1 $d1)
        ^(IF $e2 $d2)
    |   ^(IF expression fielddefines
            (^(ELIF expression fielddefines))*
            (^(ELSE fielddefines))?
        )
    ;

// for convenience in the above parser rule
fielddefines
    :   fielddefine+
    ;

kwarg
    :   ^(KWARG VARIABLENAME expression)
    ;

arguments
    :   ^(OP_CALL kwarg+)
    ;

indices
    :   ^(OP_INDEX expression+)
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
        -> { $e2.text.equals("0") }? ^(OP_LOGICAL_NOT $e1)
        -> ^(OP_EQ $e1 $e2)
    |   ^(OP_NEQ e1=expression e2=expression)
        -> { $e2.text.equals("0") }? $e1
        -> ^(OP_NEQ $e1 $e2)
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
