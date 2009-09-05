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

@includes {
    #include "pyffi/object_models/ffi/file_format.hpp"
    using namespace pyffi::object_models::ffi;
}

@members {
    // convert antlr string to std::string
    std::string get_string(pANTLR3_STRING s) {
        return std::string((const char *)s->chars); 
    };
}

ffi[FileFormat *ff]
    :   formatdefine[ff] declarations
    ;

formatdefine[FileFormat *ff]
    :   ^(FILEFORMAT 
            ^(DOC SHORTDOC*)
            (n=FORMATNAME
                {
                    ff->extensions.push_back(get_string($n.text));
                }
            )*
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
    :   class_fielddefines_ifelifelse_fragment | fielddefine+
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

