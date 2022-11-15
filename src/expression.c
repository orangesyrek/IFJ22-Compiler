#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "expression.h"
#include "lexical_analysis.h"
#include "compiler.h"

int precedence_table[8][8] = {
// +-.  */ rel cmp  (   )   id  $
	{ R , S , R , R , S , R , S , R }, // +-.	
	{ R , R , R , R , S , R , S , R }, // */
	{ S , S , X , R , S , R , S , R }, // rel
	{ S , S , S , X , S , R , S , R }, // cmp
	{ S , S , S , S , S , E , S , X }, // (
	{ R , R , R , R , X , R , X , R }, // )
	{ R , R , R , R , X , R , X , R }, // id
	{ S , S , S , S , S , X , S , X }, // $

};

expression_symbols token_to_symbol(struct lexeme token)
{
	switch (token.type)
	{
		case PLUS:
			return E_PLUS;

		case MINUS:
			return E_MINUS;

		case DOT:
			return E_CON;

		case ASTERISK:
			return E_MUL;

		case SLASH:
			return E_DIV;

		case REL_LESS:
			return E_LT;

		case REL_GREATER:
			return E_GT;

		case REL_LESS_EQ:
			return E_LEQ;

		case REL_GREATER_EQ:
			return E_GEQ;

		case REL_IDENTICAL:
			return E_EQ;

		case REL_NEQ:
			return E_NEQ;

		case L_PAR:
			return E_L_BRACKET;

		case R_PAR:
			return E_R_BRACKET;

		case STR_LIT:
		case INT_LIT:
		case DECIMAL_LIT:
		case VAR:
			return E_ID;

		default:
			return E_DOLLAR;
	}
}

// RULES
// E -> i
// E -> (E)
// E -> E + E
// E -> E - E
// E -> E . E
// E -> E * E
// E -> E / E
// E -> E === E
// E -> E !== E
// E -> E < E
// E -> E <= E
// E -> E > E
// E -> E >= E

int expression_parse (struct lexeme start_token)
{
	stack_init();
	struct lexeme current_token;

	// Where to end
	expression_symbols end_token_type;

	if (start_token.type == KEYWORD_RETURN) end_token_type = SEMICOLON;  // return ... ;
	else if (start_token.type == ASSIGNMENT) end_token_type = SEMICOLON; //      = ... ;
	else if (start_token.type == L_PAR) end_token_type = R_PAR;          //      ( ... )

	stack_push(E_DOLLAR);

	do
	{

		current_token = getToken(); // if current_token == ; or ) exit

		// Get a symbol equivalent to current token
		expression_symbols current_token_symbol = token_to_symbol(current_token);
		expression_symbols stack_top_term = stack_top_terminal();

		switch(get_op(stack_top_term, current_token_symbol))
		{
			case E:
				stack_push(current_token_symbol);
			case S:
				stack_push(S);
				stack_push(current_token_symbol);
			case R:
				reduce_rule()
			default:
				printf("ERROR");
		}

	} while (stack_top_terminal() != E_DOLLAR && current_token.type != end_token_type);

	return 0;
}

// Get operation from precedence table
expression_symbols get_op (expression_symbols stack_top, expression_symbols input)
{
	int row;
	int col;

	switch (stack_top)
	{
		case E_PLUS:
		case E_MINUS:
		case E_CON:
			row = 0;
		case E_MUL:
		case E_DIV:
			row = 1;
		case E_LT:
		case E_LEQ:
		case E_GT:
		case E_GEQ:
			row = 2;
		case E_EQ:
		case E_NEQ:
			row = 3;
		case E_L_BRACKET:
			row = 4;
		case E_R_BRACKET:
			row = 5;
		case E_ID:
			row = 6;
		case E_DOLLAR:
			row = 7;
		default:
			row = 99;
	}

	switch (input)
	{
		case E_PLUS:
		case E_MINUS:
		case E_CON:
			col = 0;
		case E_MUL:
		case E_DIV:
			col = 1;
		case E_LT:
		case E_LEQ:
		case E_GT:
		case E_GEQ:
			col = 2;
		case E_EQ:
		case E_NEQ:
			col = 3;
		case E_L_BRACKET:
			col = 4;
		case E_R_BRACKET:
			col = 5;
		case E_ID:
			col = 6;
		case E_DOLLAR:
			col = 7;
		default:
			col = 99;
	}

	if (row > 7 || col > 7) return X;
	else return precedence_table[row][col];
}

void test_rule (int count, expression_symbols one, expression_symbols two, expression_symbols three)
{

}

void reduce ()
{
	
}