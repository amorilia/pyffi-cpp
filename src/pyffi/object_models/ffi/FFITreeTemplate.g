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
    :   typedefine
        -> templatehelper(arg={$typedefine.st})
    |   parameterdefine
        -> templatehelper(arg={$parameterdefine.st})
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
    :   ^(ENUMCONSTDEF doc CONSTANTNAME expression)
        -> enumconstant(doc={$doc.st}, name={$CONSTANTNAME.text}, value={$expression.st})
    ;

classdefine
    :   ^(CLASSDEF doc TYPENAME declarations? class_fielddefines?)
        -> classdefine(doc={$doc.st}, type={$TYPENAME.text}, decls={$declarations.st}, fields={$class_fielddefines.st})
    ;

typedefine
    :   ^(TYPEDEF doc type=TYPENAME (orig=TYPENAME)?)
        -> {$orig.text != null}? aliasdefine(doc={$doc.st}, type={$type.text}, orig={$orig.text})
        -> typedefine(doc={$doc.st}, type={$type.text})
    ;

parameterdefine
    :   ^(PARAMETERDEF doc TYPENAME VARIABLENAME fieldparameters?)
        -> parameterdefine(doc={$doc.st}, type={$TYPENAME.text}, name={$VARIABLENAME.text}, kwargs={$fieldparameters.st})
    ;

fielddefine
    :   ^(FIELDDEF doc TYPENAME VARIABLENAME fieldparameters?)
        -> fielddefine(doc={$doc.st}, type={$TYPENAME.text}, name={$VARIABLENAME.text})
    ;

class_fielddefines_ifelifelse_fragment
    :   ^(IF ifexp=expression ifdefs=class_fielddefines
            (^(ELIF elifexp+=expression elifdefs+=class_fielddefines))*
            (^(ELSE elsedefs=class_fielddefines))?
        )
        -> ifelifelse(ifexp={$ifexp.st}, ifdefs={$ifdefs.st}, elifexp={$elifexp}, elifdefs={$elifdefs}, elsedefs={$elsedefs.st})
    ;

class_fielddefines
    :   class_fielddefine+
        -> templatehelper(arg={$class_fielddefines.st})
    ;

class_fielddefine
    :   class_fielddefines_ifelifelse_fragment
        -> templatehelper(arg={$class_fielddefines_ifelifelse_fragment.st})
    |   fielddefine
        -> templatehelper(arg={$fielddefine.st})
    ;

kwarg
    :   ^(FIELDARG VARIABLENAME expression)
        -> kwarg(name={$VARIABLENAME.text}, exp={$expression.st})
    ;

fieldparameters
    :   ^(FIELDARGLIST (k+=kwarg)+)
        -> fieldparameters(kwargs={$k})
    ;

expression
    :   VARIABLENAME
        -> templatehelper(arg={$VARIABLENAME.text})
    |   INT
        -> templatehelper(arg={$INT.text})
    |   FLOAT
        -> templatehelper(arg={$FLOAT.text})
    |   STRING
        -> templatehelper(arg={$STRING.text})
    |   ^(OP_LOGICAL_OR e1=expression e2=expression)
        -> op_logical_or(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_LOGICAL_AND e1=expression e2=expression)
        -> op_logical_and(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_LOGICAL_NOT expression)
        -> op_logical_not(e1={$e1.st})
    |   ^(OP_EQ e1=expression e2=expression)
        -> op_eq(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_NEQ e1=expression expression)
        -> op_neq(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_GT e1=expression e2=expression)
        -> op_gt(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_LT e1=expression e2=expression)
        -> op_lt(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_GTEQ e1=expression e2=expression)
        -> op_gteq(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_LTEQ e1=expression e2=expression)
        -> op_lteq(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_BITWISE_OR e1=expression e2=expression)
        -> op_bitwise_or(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_BITWISE_AND e1=expression e2=expression)
        -> op_bitwise_and(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_LEFTSHIFT e1=expression e2=expression)
        -> op_left_shift(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_RIGHTSHIFT e1=expression e2=expression)
        -> op_right_shift(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_PLUS e1=expression e2=expression)
        -> op_plus(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_MINUS e1=expression e2=expression)
        -> op_minus(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_NEGATE e1=expression)
        -> op_negate(e1={$e1.st})
    |   ^(OP_BITWISE_NOT e1=expression)
        -> op_bitwise_not(e1={$e1.st})
    |   ^(OP_MULTIPLY e1=expression e2=expression)
        -> op_multiply(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_DIVIDE e1=expression e2=expression)
        -> op_divide(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_MODULO e1=expression e2=expression)
        -> op_modulo(e1={$e1.st}, e2={$e2.st})
    |   ^(OP_POWER e1=expression e2=expression)
        -> op_power(e1={$e1.st}, e2={$e2.st})
    ;
