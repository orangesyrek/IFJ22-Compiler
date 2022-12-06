/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: lexical_analysis.h
 * Author(s): xbarta50, xjanot04
 */

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
    REL_NEQ,
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
    ERROR_LEX, /* ERROR STATE FOR LEXEME */
} lex_types;

typedef enum {
    Start, /**/
    SLASH_STATE, /* / */
    ONE_LINE_COMMENT, /* // */
    BACKSLASH, /* /\ */
    COMMENT_END, // //\n || /**/
    MULTI_LINE_COMMENT, /**/
    MULTIPLY, /* * */
    VAR_PREFIX, /* $ */
    VAR_STATE, /* $variableName_123 */
    SEMICOLON_STATE, /* ; */
    R_CURLY_STATE,   /* } */
    L_CURLY_STATE, /* { */
    REL_LESS_STATE, /* < */
    REL_LESS_EQ_STATE, /* <= */
    PROLOG_STATE, /* <?php */
    REL_GREATER_STATE, /* > */
    REL_GREATER_EQ_STATE,/* >= */
    PLUS_STATE, /* + */
    MINUS_STATE, /* - */
    ASTERISK_STATE, /* * */
    DOT_STATE, /* . */
    COMMA_STATE, /* , */
    COLON_STATE, /* : */
    L_PAR_STATE, /* ( */
    R_PAR_STATE,  /* ) */
    QUESTION_MARK, /* ? */
    PROLOG_END_STATE, /* ?> */
    FUN_ID_STATE, /* variableName_123 */
    INT_LIT_STATE, /* 0-9 */
    INT_LIT_DOT, /* 0-9.0-9 */
    INT_LIT_E, /* 0-9e/E */
    DEC_LIT_TMP, /* 0-9.0-9 */
    INT_LIT_E_SIGN, /* 0-9eE+- */
    DEC_LIT_E_TMP, /* 0-9eE+-0-9 */
    STR_LIT_BEGIN, /* "abcd */
    STR_LIT_STATE, /* "abcd" */
    STR_LIT_ESCAPE, /* "abcd\ */
    ASSIGNMENT_STATE, /* = */
    DOUBLEASSIGNMENT, /* == */
    REL_IDENTICAL_STATE, /* ==== */
    EXCLAMATION_MARK, /* ! */
    EXCLAMATION_MARK_EQUAL, /* != */
    REL_NEQ_STATE, /* !== */
    LEX_EOF_STATE, /* EOF */
    PROLOG_FIRST, /* <? */
    PROLOG_SECOND, /* <?p */
    PROLOG_THIRD, /* <?ph */
    PROLOG_FOURTH, /* <?php */
    QUESTION_MARK_i, /* ?i */
    QUESTION_MARK_i_n, /* ?in */
    QUESTION_MARK_i_n_t, /* ?int */
    QUESTION_MARK_f, /* ?f */
    QUESTION_MARK_f_l, /* ?fl */
    QUESTION_MARK_f_l_o, /* ?flo */
    QUESTION_MARK_f_l_o_a, /* ?floa */
    QUESTION_MARK_f_l_o_a_t, /* ?float */
    QUESTION_MARK_s, /* ?s */
    QUESTION_MARK_s_t, /* ?st */
    QUESTION_MARK_s_t_r, /* ?str */
    QUESTION_MARK_s_t_r_i,  /* ?stri */
    QUESTION_MARK_s_t_r_i_n, /* ?strin */
    QUESTION_MARK_s_t_r_i_n_g, /* ?string */
    ERROR_STATE,
} state;

struct lexeme {
    lex_types type; /**< type of token */
    char *id;       /**< token's identificator if it has any */

    union {         /**< token's value based on it's type */
        int int_val;
        double flt_val;
        char *str_val;
    } value;
};



void printToken(struct lexeme lex); /*Help function to print what lexeme's we have*/
void print_ctx();

/**
 * @brief Gets the next token from the input and set it's type and data based on an automata.
 *
 * @return A token.
 */
struct lexeme getToken();

#endif
