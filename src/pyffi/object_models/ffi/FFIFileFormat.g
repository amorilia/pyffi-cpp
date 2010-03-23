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

    FileFormat *_fileformat;
}

ffi[FileFormat *ff]
@init {
    _fileformat = ff;
}
    :   formatdefine declarations
    ;

formatdefine
    :   ^(FILEFORMAT 
            ^(DOC SHORTDOC*)
            (n=CONSTANTNAME
                {
                    _fileformat->extensions.push_back(get_string($n.text));
                }
            )*
        )
    ;

declarations
    :   (typedefine | parameterdefine | classdefine | enumdefine)*
    ;

enumdefine
    :   ^(ENUMDEF ^(DOC SHORTDOC*) TYPENAME TYPENAME enumconstant+);

enumconstant
    :   ^(ENUMCONSTDEF ^(DOC SHORTDOC*) CONSTANTNAME INT);

classdefine
    :   ^(CLASSDEF ^(DOC SHORTDOC*) TYPENAME ^(BASE TYPENAME?) declarations class_fielddefines)
    ;

typedefine
    :   ^(TYPEDEF ^(DOC SHORTDOC*) TYPENAME TYPENAME?)
    ;

parameterdefine
    :   ^(PARAMETERDEF ^(DOC SHORTDOC*) TYPENAME VARIABLENAME arguments?)
    ;

fielddefine
    :   ^(FIELDDEF ^(DOC SHORTDOC*) TYPENAME VARIABLENAME arguments?)
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
