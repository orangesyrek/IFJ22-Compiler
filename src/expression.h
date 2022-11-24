#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lexical_analysis.h"

typedef enum {

	// Expression symbols
	E_PLUS,   		// +
	E_MINUS,  		// -
	E_CON,   		  // .
	E_MUL,    		// *
	E_DIV,    		// /
	E_LT,     		// <
	E_GT,     		// >
	E_LEQ,    		// <=
	E_GEQ,    		// >=
	E_EQ,     		// ===
	E_NEQ,    		// !==
	E_L_BRACKET, 	// (
	E_R_BRACKET, 	// )
	E_ID,        	// i
	E_DOLLAR,    	// $

	E_NON_TERM,

	// Precedence table actions
	R, // >
	S, // <
	E, // =
	X, // error
} expression_symbols;

typedef enum {

	E_TO_I,      // E -> i
	LBRA_E_RBRA, // E -> (E)
	E_PLUS_E,    // E -> E + E
	E_MINUS_E,   // E -> E - E
	E_CON_E,     // E -> E . E
	E_MUL_E,     // E -> E * E
	E_DIV_E,     // E -> E / E
	E_LT_E,      // E -> E < E
	E_GT_E,      // E -> E > E
	E_LEQ_E,     // E -> E <= E
	E_GEQ_E,     // E -> E >= E
	E_EQ_E,      // E -> E === E
	E_NEQ_E,     // E -> E !== E
	NOT_A_RULE   // Wrong rule

} rules;

typedef struct item {
	expression_symbols symbol;
	lex_types type;
	struct lexeme token;
} stack_item;

expression_symbols token_to_symbol (struct lexeme token);

expression_symbols get_op (expression_symbols stack_top, expression_symbols input);

int test_rule (int count, stack_item one, stack_item two, stack_item three);

int reduce ();

int test_semantics (rules rule, stack_item one, stack_item two, stack_item three, lex_types *non_term_type);

int expression_parse (struct lexeme start_token, struct lexeme first_token);

#endif