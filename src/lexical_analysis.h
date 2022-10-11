#ifndef LEX_ANALYSIS_H
#define LEX_ANALYSIS_H

typedef enum {
	FUN_ID = 0,	/* foo */
	STR_LIT,	/* "ab" */
	ASSIGNMENT,	/* = */
	INT_LIT,	/* 42 */
	DECIMAL_LIT,	/* 42.5, 42e+10,42.5E-2 */
	PARAM_TYPE,	/* int, ?int */
	VAR,		/* $x */
	L_PAR,		/* ( */
	R_PAR,		/* ) */
	SEMICOLON,	/* ; */
	L_CURLY,	/* { */
	R_CURLY,	/* } */
	REL_IDENTICAL,	/* === */
	REL_NOT_IDENTIKAL, /* !== /*
	REL_LESS,	/* < */
	REL_LESS_EQ,	/* <= */
	REL_GREATER,	/* > */
	REL_GREATER_EQ,	/* >= */
	PLUS,	/* + */
	MINUS,	/* - */
	ASTERISK,	/* * */
	SLASH,	/* / */
	DOT,	/* . */
	COMMA,	/* , */
	COLON,	/* : */
	EXCLAMATION_MARK,	/* ! */
	NL,	/* \n */
	KEYWORD_IF,	/* if */
	KEYWORD_ELSE,	/* else */
	KEYWORD_FUNCTION,	/* function */
	KEYWORD_NULL,	/* null */
	KEYWORD_RETURN,	/* return */
	KEYWORD_VOID,	/* void */
	KEYWORD_WHILE,	/* while */
	KEYWORD_DECLARE, /* declare */
	KEYWORD_STRICT_TYPES, /* strict_types */
	COMMENT,	/* */
	PROLOG_END, /* ?> */
	LEX_EOF,	/* EOF TODO*/
} lex_types;

void print_ctx();

#endif
