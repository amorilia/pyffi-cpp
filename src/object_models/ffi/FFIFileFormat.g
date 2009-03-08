// this grammar converts an FFI AST into a FileFormat instance
tree grammar FFIFileFormat;

options {
    language=C;
    output=AST;
    ASTLabelType=pANTLR3_BASE_TREE;
    tokenVocab=FFI;
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

scope GlobalScope {
    FileFormat *ff;
}

@includes {
    #include "pyffi/object_models/ffi/file_format.hpp"
	using namespace pyffi::object_models::ffi;
}

ffi[FileFormat *ff]
scope GlobalScope;
@init {
    $GlobalScope::ff = ff;
}
    :   formatdefine declarations
    ;

formatdefine
scope GlobalScope;
    :   ^(FILEFORMAT 
            ^(DOC SHORTDOC*)
            (name=FORMATNAME {$GlobalScope::ff->extensions.push_back((const char *)$name.text->chars);})+
        )
    ;

declarations
    :   (typeblock | parameterblock | classblock)*
    ;

typeblock
    :   ^(TYPE typedefine+)
    ;

parameterblock
    :   ^(PARAMETER fielddefine+)
    ;

classblock
    :   ^(CLASS ^(DOC SHORTDOC*) TYPENAME declarations class_fielddefines)
    ;

typedefine
    :   ^(TYPEDEF ^(DOC SHORTDOC*) TYPENAME TYPENAME?)
    ;

fielddefine
    :   ^(FIELDDEF ^(DOC SHORTDOC*) TYPENAME VARIABLENAME fieldparameters?)
    ;

class_fielddefines_ifelifelse_fragment
    :   ^(IF ifexp=expression ifdefines=class_fielddefines
          ^(ELIF elifexp=expression elifdefines=class_fielddefines)
          ^(ELSE elsedefines=class_fielddefines))
    ;

class_fielddefines
    :   ^(FIELD (class_fielddefines_ifelifelse_fragment | fielddefine)+)
    ;

kwarg
    :   ^(FIELDARG VARIABLENAME expression)
    ;

fieldparameters
    :   ^(FIELDARGLIST kwarg+)
    ;

// TODO: operators such as and, or, not, ...
expression
    :   VARIABLENAME
    |   INT
    |   FLOAT
    |   STRING
    ;

