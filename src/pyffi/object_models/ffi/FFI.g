grammar FFI;

options {
    language=C;
    output=AST;
    ASTLabelType=pANTLR3_BASE_TREE;
}

tokens {
    // emitted when indentation increases
    // see NEWLINE for details
    INDENT;
    // emitted when indentation decreases
    // see NEWLINE for details
    DEDENT;

    DOC;
    FIELDDEF;
    KWARG;
    CLASS='class';
    CLASSDEF;
    BASE;
    ELIF='elif';
    ELSE='else';
    FILEFORMAT='fileformat';
    IF='if';
    ABSTRACT='abstract';
    ENUM='enum';
    ENUMDEF;
    ENUMCONSTDEF;
    // OP_MINUS is converted into OP_NEGATE if it corresponds to negation
    OP_NEGATE;
    OP_INDEX='[';
    OP_CALL='(';
}

// target specific code
// ====================

// current_indent: lexer variable which measures the current indentation level
// NEWLINE: special lexer token which works as follows
//    for every new line: emit NEWLINE
//    indentation increased by one level: emit INDENT
//    indentation decreased (by any level): emit DEDENT as many as decreased

// to allow multiple tokens to be emitted by the NEWLINE lexer, the emit
// and nextToken members are customized (this is described on the ANTLR wiki)

// JAVA

/*

@lexer::members {
    // keep track of indentation
    int current_indent = 0;
    
    // allow multiple tokens to be emitted
    // (see http://www.antlr.org/wiki/pages/viewpage.action?pageId=3604497)
    List tokens = new ArrayList();
    public void emit(Token token) {
        state.token = token;
        tokens.add(token);
    }
    public Token nextToken() {
        super.nextToken();
        if ( tokens.size()==0 ) {
            return Token.EOF_TOKEN;
        }
        return (Token)tokens.remove(0);
    }
}

NEWLINE
@init {
    int indent = 0;
}
    :
        (
            (('\f')? ('\r')? '\n'
                {
                    emit(new ClassicToken(NEWLINE, "\n"));
                }
            )
            |
            ' ')*
        (('\f')? ('\r')? '\n'
            {
                emit(new ClassicToken(NEWLINE, "\n"));
            }
        )
        ('    '
        {
            indent++;
        }
        )*
        {
            if (indent == current_indent + 1) {
                current_indent++;
                emit(new ClassicToken(INDENT, ">"));
            }
            else if (indent == current_indent) {
                // nothing happens, newline already emitted
            }
            else if (indent < current_indent) {
                while (indent < current_indent) {
                    current_indent--;
                    emit(new ClassicToken(DEDENT, "<"));
                }
            }
            else {
                throw new RuntimeException("bad indentation");
            }
        }
    ;

*/

// PYTHON
// note: ANTLR defines Python members on class level, but we want to define an
//       instance variable, not a class variable, hence it must go in __init__
//       so we declare the member in __init__

/*

@lexer::init {
    self.current_indent = 0
    self.token_stack = []
}

// multiple tokens per lexical symbol

@lexer::members {
    def emit(self, token=None):
        # call the base class method
        Lexer.emit(self, token)
        # append token to stack
        self.token_stack.append(self._state.token)

    def nextToken(self):
        # call the base class method
        Lexer.nextToken(self)
        try:
            # pop token from stack
            return self.token_stack.pop(0)
        except IndexError:
            # pop from empty list raises index error
            # flag end of file
            return EOF_TOKEN
}

NEWLINE
@init {
    indent = 0
}
    :
        (
            (('\f')? ('\r')? '\n'
                {
                    self.emit(ClassicToken(NEWLINE, "\n"));
                }
            )
            |
            ' ')*
        (('\f')? ('\r')? '\n'
            {
                self.emit(ClassicToken(NEWLINE, "\n"));
            }
        )
        ('    '
            {
                indent += 1
            }
        )*
        {   
            if (indent == self.current_indent + 1):
                self.current_indent += 1
                self.emit(ClassicToken(INDENT, ">"))
            elif indent == self.current_indent:
                pass
            elif indent < self.current_indent:
                while indent < self.current_indent:
                    self.current_indent -= 1
                    self.emit(ClassicToken(DEDENT, "<"))
            else:
                raise RuntimeError("bad indentation")
        }
    ;

*/

// C

@lexer::members {
    static int current_indent = 0;

    // helper function to create token in lexer
    // (this is adapted from the createToken function in 
    pANTLR3_COMMON_TOKEN createLexerToken(pANTLR3_LEXER lexer, ANTLR3_UINT32 tokenType, const char * text)
    {
        pANTLR3_COMMON_TOKEN    newToken;
        
        newToken = lexer->rec->state->tokFactory->newToken(lexer->rec->state->tokFactory);
        
        if (newToken != NULL)
        {
            newToken->type = tokenType;
            newToken->input = lexer->rec->state->tokFactory->input;
            if (text != NULL) {
                newToken->textState = ANTLR3_TEXT_CHARP;
                newToken->tokText.chars = (pANTLR3_UCHAR)text;
            } else {
                newToken->textState = ANTLR3_TEXT_NONE;
            };
        }
        return  newToken;
    }
}

NEWLINE
@init {
    int indent = 0;
    pANTLR3_LEXER lexer = ctx->pLexer;
}
    :
        (
            (('\f')? ('\r')? '\n'
                {
                    lexer->emitNew(lexer, createLexerToken(lexer, NEWLINE, "\n"));
                }
            )
            |
            ' ')*
        (('\f')? ('\r')? '\n'
            {
                lexer->emitNew(lexer, createLexerToken(lexer, NEWLINE, "\n"));
            }
        )
        ('    '
            {
                indent++;
            }
        )*
        {   
            if (indent == current_indent + 1) {
                current_indent++;
                lexer->emitNew(lexer, createLexerToken(lexer, INDENT, ">"));
            } else if (indent == current_indent) {
            } else if (indent < current_indent) {
                do {
                    current_indent--;
                    lexer->emitNew(lexer, createLexerToken(lexer, DEDENT, "<"));
                } while (indent < current_indent);
            } else {
                ANTLR3_FPRINTF(stderr, "NEWLINE: Error: Bad indentation!\n");
                // TODO: set lexer error state
            };
        }
    ;

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

ffi
    :   formatdefine declarations? EOF!
    ;

formatdefine
    :   longdoc? FILEFORMAT (CONSTANTNAME (',' CONSTANTNAME)*)? SHORTDOC? NEWLINE+
        -> ^(FILEFORMAT ^(DOC longdoc? SHORTDOC?) CONSTANTNAME*)
    ;

declarations
    :   (fielddefine | classdefine | enumdefine)+
    ;

// Documentation preceeding a definition, with single newline
// following each line of text. The number of lines is arbitrary.
longdoc
    :   (SHORTDOC NEWLINE!)+
    ;

enumdefine
    :   longdoc? ENUM name=TYPENAME '(' base=TYPENAME ')' blockbegin enumconstant+ blockend
        -> ^(ENUMDEF ^(DOC longdoc?) $name $base enumconstant+);

enumconstant
    :   longdoc? CONSTANTNAME '=' INT SHORTDOC? NEWLINE+
        -> ^(ENUMCONSTDEF ^(DOC longdoc? SHORTDOC?) CONSTANTNAME INT);

classdefine
    :   longdoc? CLASS name=TYPENAME ('(' base=TYPENAME ')')? (blockbegin declarations blockend|NEWLINE+)
        -> ^(CLASSDEF ^(DOC longdoc?) $name ^(BASE $base?) declarations?)
    ;

blockbegin
    :   COLON NEWLINE+ INDENT
    ;

blockend
    :   DEDENT
    ;

fielddefine
    :   longdoc? ABSTRACT? TYPENAME VARIABLENAME fieldindices? fieldarguments? SHORTDOC? NEWLINE+
        -> ^(FIELDDEF ^(DOC longdoc? SHORTDOC?) TYPENAME VARIABLENAME fieldindices? fieldarguments? ABSTRACT?)
    |   IF^ expression blockbegin! fielddefines blockend! fieldelif* fieldelse?
    ;

fieldelif
    :   ELIF^ expression blockbegin! fielddefines blockend!
    ;

fieldelse
    :   ELSE^ blockbegin! fielddefines blockend!
    ;

fielddefines
    :   fielddefine+
    ;

kwarg
    :   VARIABLENAME '=' expression
        -> ^(KWARG VARIABLENAME expression)
    ;

expression
	:   or_test
    ;

or_test
	:   and_test (OP_LOGICAL_OR^ and_test)*
    ;

and_test
    :   not_test (OP_LOGICAL_AND^ not_test)*
    ;

not_test
	:   OP_LOGICAL_NOT^ not_test
	|   comparison
	;

comparison
    :   or_expr (comp_op^ or_expr)*
    ;

comp_op
	:   OP_EQ
	|   OP_NEQ
	|   OP_GT
	|   OP_LT
	|   OP_GTEQ
	|   OP_LTEQ
	;

or_expr
    :   and_expr (OP_BITWISE_OR^ and_expr)*
	;

and_expr
    :   shift_expr (OP_BITWISE_AND^ shift_expr)*
	;

shift_expr
    :   arith_expr ((OP_LEFTSHIFT^|OP_RIGHTSHIFT^) arith_expr)*
	;

arith_expr
    :   term ((OP_PLUS^|OP_MINUS^) term)*
	;

term
    :   factor ((OP_MULTIPLY^|OP_DIVIDE^|OP_MODULO^) factor)*
	;

factor
    :   OP_PLUS! factor
    |   op=OP_MINUS factor
        -> ^(OP_NEGATE[$op, "-"] factor)
    |   OP_BITWISE_NOT^ factor
	|   power
	;

power
    :   arguments (options {greedy=true;}:OP_POWER^ factor)?
    ;

arguments
    :   indices (OP_CALL^ kwarg (COMMA! kwarg)* OP_CALL_END!)?
    ;
    
indices
    :   atom (OP_INDEX^ expression OP_INDEX_END! (OP_INDEX! expression OP_INDEX_END!)*)?
    ;

fieldarguments
    :   OP_CALL^ kwarg (COMMA! kwarg)* OP_CALL_END!
    ;
    
fieldindices
    :   OP_INDEX^ expression OP_INDEX_END! (OP_INDEX! expression OP_INDEX_END!)*
    ;

atom
    :   VARIABLENAME
    |   INT
    |   FLOAT
    |   STRING
    |   OP_CALL! expression OP_CALL_END!
    ;

/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/

// whitespace and comments

// documentation following a definition, no newline
// (on the same line of the definition)
SHORTDOC
    :   '#' ~('\n')*
    ;

fragment
DIGIT
    :   '0'..'9'
    ;

fragment
DIGITS  :   DIGIT+
    ;

INT
    // hex
    :   '0x' ( '0' .. '9' | 'a' .. 'f' | 'A' .. 'F' )+
    // octal
    |   '0o' ( '0' .. '7')+
    // binary
    |   '0b' ( '0' .. '1')+
    // decimal
    |   DIGITS+
    ;

fragment
EXPONENT
    :    'e' ( '+' | '-' )? DIGITS
    ;

FLOAT
    :   DIGITS '.' DIGITS EXPONENT?
    ;

COLON
    :   ':'
    ;

OP_EQ
    :   '==';

OP_NEQ
    :   '!=';

OP_RIGHTSHIFT
    :   '>>';

OP_LEFTSHIFT
    :   '<<';

OP_GTEQ
    :   '>=';

OP_LTEQ
    :   '<=';

OP_GT
    :   '>';

OP_LT
    :   '<';

OP_LOGICAL_NOT
    :   'not';

OP_BITWISE_NOT
    :   '~';

OP_LOGICAL_AND
    :   'and';

OP_BITWISE_AND
    :   '&';

OP_LOGICAL_OR
    :   'or';

OP_BITWISE_OR
    :   '|';

OP_POWER
    :   '**';

OP_MINUS
    :   '-';

OP_PLUS
    :   '+';

OP_MULTIPLY
    :   '*';

OP_DIVIDE
    :   '/';

OP_MODULO
    :   '%';

OP_CALL_END
    :   ')';

OP_INDEX_END
    :   ']';

COMMA
    :   ',';

fragment
LCLETTER
    :   'a'..'z'
    ;

fragment
UCLETTER
    :   'A'..'Z'
    ;

// constant name (this identifies the file extension)
CONSTANTNAME
    :   (UCLETTER | DIGITS | '_')+
    ;

// lower_case_with_underscores for variable names (e.g. fields)
VARIABLENAME
    :   LCLETTER (LCLETTER | DIGITS | '_')*
    ;

// CamelCase for type names
TYPENAME
    :   UCLETTER (LCLETTER | UCLETTER | DIGITS)*
    ;

STRING
    :   '"' (ESC|~('\\'|'\n'|'"'))* '"'
    ;

fragment
ESC
    :   '\\' .
    ;

// ignore whitespace
WS
    :   (' ')+ { $channel=HIDDEN; }
    ;
