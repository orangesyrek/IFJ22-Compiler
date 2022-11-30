#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "lexical_analysis.h"
#include "compiler.h"

/**
 * @enum Symbols used by stack
 */
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

/**
 * @enum rules
 */
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

/**
 * @struct Stack item
 */
typedef struct item {
	expression_symbols symbol;
	lex_types type;
	struct lexeme token;
} stack_item;

/**
 * @brief      Converts token to symbol that can later be used by stack
 *
 * @param[in]  token Token to be converted
 *
 * @return     Returns symbol corresponding to the given token
 */
expression_symbols token_to_symbol (struct lexeme token);

/**
 * @brief      Gets the operation based on the top of the stack and the current token symbol
 *
 * @param[in]  stack_top  Symbol of the terminal on the top of the stack
 * @param[in]  input      Symbol of the current token
 *
 * @return     Returns the operation
 */
expression_symbols get_op (expression_symbols stack_top, expression_symbols input);

/**
 * @brief      Check what rule should be applied based on the three top terminals on the stack
 *
 * @param[in]  count  Number of terminals
 * @param[in]  one    First terminal
 * @param[in]  two    Second terminal
 * @param[in]  three  Third terminal
 *
 * @return     Corresponding rule
 */
rules test_rule (int count, stack_item one, stack_item two, stack_item three);

/**
 * @brief      Reduces the terminals on stack based on the rule from function test_rule
 *
 * @return     Returns COMP_OK if succesfull, other errors (based on the issue) when not 
 */
comp_err reduce ();

/**
 * @brief      Partially tests the semantics of the operations, also calls functions responsible for generating code
 *
 * @param[in]  rule           The rule to be tested
 * @param[in]  one            First terminal from the stack
 * @param[in]  two            Second terminal from the stack
 * @param[in]  three          Third terminal from the stack
 * @param      non_term_type  Pointer to a variable storing the results type (int, float, string)
 *
 * @return     Returns COMP_OK if succesfull, other errors (based on the issue) when not
 */
comp_err test_semantics (rules rule, stack_item one, stack_item two, stack_item three, lex_types *non_term_type);

/**
 * @brief      Main function which parses the expression
 *
 * @param[in]  start_token  The start token
 * @param[in]  first_token  The first token to be parsed
 *
 * @return     Returns COMP_OK if succesfull, other errors (based on the issue) when not
 */
comp_err expression_parse (struct lexeme start_token, struct lexeme first_token);

#endif