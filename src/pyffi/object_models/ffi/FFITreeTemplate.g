/* 
  Tree grammar for FFI, with templates.
*/
tree grammar FFITreeTemplate;

options {
    output=template;
    tokenVocab=FFI;
    ASTLabelType=CommonTree;
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

ffi
    :   formatdefine declarations?
        -> ffi(head={$formatdefine.st}, decls={$declarations.st})
    ;

doc
    :   ^(DOC (docstr+=SHORTDOC)*)
        -> doc(docstrings={$docstr})
    ;

formatdefine
    :   ^(FILEFORMAT 
            doc
            (n+=CONSTANTNAME)*
        )
        -> formatdefine(doc={$doc.st}, names={$n})
    ;

declarations
    :   (decls+=declaration)+
        -> templatehelper(arg={$decls})
    ;

declaration
    :   fielddefine
        -> templatehelper(arg={$fielddefine.st})
    |   classdefine
        -> templatehelper(arg={$classdefine.st})
    |   enumdefine
        -> templatehelper(arg={$enumdefine.st})
    ;

enumdefine
    :   ^(ENUMDEF doc
          name=TYPENAME type=TYPENAME
          (constants+=enumconstant)+
        )
        -> enumdefine(doc={$doc.st}, name={$name.text}, type={$type.text}, constants={$constants})
    ;

enumconstant
    :   ^(ENUMCONSTDEF doc CONSTANTNAME expression[999])
        -> enumconstant(doc={$doc.st}, name={$CONSTANTNAME.text}, value={$expression.st})
    ;

classdefine
    :   ^(CLASSDEF doc name=TYPENAME ^(BASE (base=TYPENAME)?) declarations?)
        -> {$base.text != null}? classdefine(doc={$doc.st}, type={$name.text}, base={$base.text}, decls={$declarations.st})
        -> classdefine(doc={$doc.st}, type={$name.text}, decls={$declarations.st})
    ;

fielddefine
    :   ^(FIELDDEF doc TYPENAME VARIABLENAME indices? arguments?)
        -> fielddefine(doc={$doc.st}, type={$TYPENAME.text}, name={$VARIABLENAME.text}, indices={$indices.st}, kwargs={$arguments.st})
    |   ^(IF ifexp=expression[999] ifdefs=fielddefines
            (^(ELIF elifexp+=expression[999] elifdefs+=fielddefines))*
            (^(ELSE elsedefs=fielddefines))?
        )
        -> ifelifelse(ifexp={$ifexp.st}, ifdefs={$ifdefs.st}, elifexp={$elifexp}, elifdefs={$elifdefs}, elsedefs={$elsedefs.st})
    ;

// for convenience in fielddefine rule
fielddefines
    :   (defs+=fielddefine)+
        -> templatehelper(arg={$defs})
    ;

arguments
    :   ^(OP_CALL (k+=kwarg)+)
        -> arguments(kwargs={$k})
    ;
    
indices
    :   ^(OP_INDEX (exps+=expression[999])+)
        -> indices(expressions={$exps})
    ;

kwarg
    :   ^(KWARG VARIABLENAME expression[999])
        -> kwarg(name={$VARIABLENAME.text}, exp={$expression.st})
    ;

// pass precedence of enclosing expression (to decide on brackets)
expression[int precedence]
    :   VARIABLENAME
        -> templatehelper(arg={$VARIABLENAME.text})
    |   INT
        -> templatehelper(arg={$INT.text})
    |   FLOAT
        -> templatehelper(arg={$FLOAT.text})
    |   STRING
        -> string(text={$STRING.text})
    |   ^(OP_LOGICAL_OR e1=expression[140] e2=expression[140])
        -> op_logical_or(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 140})
    |   ^(OP_LOGICAL_AND e1=expression[130] e2=expression[130])
        -> op_logical_and(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 130})
    |   ^(OP_LOGICAL_NOT e1=expression[120])
        -> op_logical_not(e1={$e1.st}, brackets={$precedence < 120})
    |   ^(OP_EQ e1=expression[110] e2=expression[110])
        -> op_eq(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 110})
    |   ^(OP_NEQ e1=expression[110] e2=expression[110])
        -> op_neq(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 110})
    |   ^(OP_GT e1=expression[110] e2=expression[110])
        -> op_gt(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 110})
    |   ^(OP_LT e1=expression[110] e2=expression[110])
        -> op_lt(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 110})
    |   ^(OP_GTEQ e1=expression[110] e2=expression[110])
        -> op_gteq(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 110})
    |   ^(OP_LTEQ e1=expression[110] e2=expression[110])
        -> op_lteq(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 110})
    |   ^(OP_BITWISE_OR e1=expression[100] e2=expression[100])
        -> op_bitwise_or(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 100})
    |   ^(OP_BITWISE_AND e1=expression[90] e2=expression[90])
        -> op_bitwise_and(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 90})
    |   ^(OP_LEFTSHIFT e1=expression[80] e2=expression[80])
        -> op_left_shift(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 80})
    |   ^(OP_RIGHTSHIFT e1=expression[80] e2=expression[80])
        -> op_right_shift(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 80})
    |   ^(OP_PLUS e1=expression[70] e2=expression[70])
        -> op_plus(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 70})
    |   ^(OP_MINUS e1=expression[70] e2=expression[70])
        -> op_minus(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 70})
    |   ^(OP_NEGATE e1=expression[50])
        -> op_negate(e1={$e1.st}, brackets={$precedence < 50})
    |   ^(OP_BITWISE_NOT e1=expression[50])
        -> op_bitwise_not(e1={$e1.st}, brackets={$precedence < 50})
    |   ^(OP_MULTIPLY e1=expression[60] e2=expression[60])
        -> op_multiply(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 60})
    |   ^(OP_DIVIDE e1=expression[60] e2=expression[60])
        -> op_divide(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 60})
    |   ^(OP_MODULO e1=expression[60] e2=expression[60])
        -> op_modulo(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 60})
    |   ^(OP_POWER e1=expression[40] e2=expression[40])
        -> op_power(e1={$e1.st}, e2={$e2.st}, brackets={$precedence < 40})
    ;
