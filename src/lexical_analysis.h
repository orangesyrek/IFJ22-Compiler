#ifndef LEX_ANALYSIS_H
#define LEX_ANALYSIS_H

typedef enum {
    FUN_ID = 0,    /* foo */
    STR_LIT,    /* "ab" */
    ASSIGNMENT,    /* = */
    INT_LIT,    /* 42 */
    DECIMAL_LIT,    /* 42.5, 42e+10,42.5E-2 */
    VAR,        /* $x */
    L_PAR,        /* ( */
    R_PAR,        /* ) */
    SEMICOLON,    /* ; */
    L_CURLY,    /* { */
    R_CURLY,    /* } */
    REL_IDENTICAL,    /* === */
    REL_LESS,    /* < */
    REL_LESS_EQ,    /* <= */
    REL_GREATER,    /* > */
    REL_GREATER_EQ,    /* >= */
    PLUS,    /* + */
    MINUS,    /* - */
    ASTERISK,    /* * */
    SLASH,    /* / */
    DOT,    /* . */
    COMMA,    /* , */
    COLON,    /* : */
    PARAM_TYPE,    /* int, ?int */
    COMMENT,    /* */
    PROLOG_START,	/* <?php */
    PROLOG_END, /* ?> */
    LEX_EOF,    /* EOF */
    KEYWORD_IF,    /* if */
    KEYWORD_ELSE,    /* else */
    KEYWORD_FUNCTION,    /* function */
    KEYWORD_NULL,    /* null */
    KEYWORD_RETURN,    /* return */
    KEYWORD_VOID,    /* void */
    KEYWORD_WHILE,    /* while */
    KEYWORD_DECLARE, /* declare */
    KEYWORD_STRICT_TYPES, /* strict_types */
} lex_types;

typedef enum {
    Start, /**/
    SLASH, /* / */
    ONE_LINE_COMMENT,
    BACKSLASH, /* \ */
    COMMENT_END,
    MULTI_LINE_COMMENT,
    MULTIPLY, /* * */
    VAR_PREFIX,
    VAR,
    SEMICOLON, /* ; */
    R_CURLY,   /* } */
    L_CURLY, /* { */
    REL_LESS, /* < */
    REL_LESS_EQ, /* <= */
    PROLOG, /* <?php */
    REL_GREATER, /* > */
    REL_GREATER_EQ,/* >= */
    PLUS,
    MINUS,
    ASTERISK,
    DOT,
    COMMA,
    COLON,
    L_PAR, /* ( */
    R_PAR, 
    QUESTION_MARK,
    PROLOG_END,
    FUN_ID,
    INT_LIT,
    INT_LIT_DOT,
    INT_LIT_E,
    DEC_LIT_TMP,
    INT_LIT_E_SIGN,
    DECIMAL_LIT,
    DEC_LIT_E_TMP,
    STR_LIT_BEGIN,
    STR_LIT,
    STR_LIT_ESCAPE,
    ASSIGNMENT,
    DOUBLEASSIGNMENT, /* == */
    REL_IDENTICAL, /* ==== */
    EXCLAMATION_MARK, /* ! */
    EXCLAMATION_MARK_EQUAL, /* != */
    REL_NEQ, /* !== */
    LEX_EOF, /* EOF */ //45
    PROLOG_FIRST,
    PROLOG_SECOND,
    PROLOG_THIRD,
    ERROR,
} state;

struct lexeme {
    lex_types type;
    char *id;

    union {
        int int_val;
        double flt_val;
        char *str_val;
    } value;

    unsigned int symtab_index;
};

void print_ctx();

#endif