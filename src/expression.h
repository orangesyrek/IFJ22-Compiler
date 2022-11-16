#ifndef EXPRESSION_H
#define EXPRESSION_H

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

expression_symbols get_op (expression_symbols stack_top, expression_symbols input);

expression_symbols token_to_symbol (struct lexeme token);

int expression_parse(struct lexeme start_token);

void test_rule (int count, expression_symbols one, expression_symbols two, expression_symbols three);

void reduce ();

#endif