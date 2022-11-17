#include <stdlib.h>
#include <stdio.h>

#include "compiler.h"
#include "stack.h"
#include "expression.h"

extern int top;
extern expression_symbols stack[MAX_STACK_SIZE];

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

expression_symbols token_to_symbol (struct lexeme token)
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
		case KEYWORD_NULL:
			return E_ID;

		default:
			return E_DOLLAR;
	}
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
			break;
		case E_MUL:
		case E_DIV:
			row = 1;
			break;
		case E_LT:
		case E_LEQ:
		case E_GT:
		case E_GEQ:
			row = 2;
			break;
		case E_EQ:
		case E_NEQ:
			row = 3;
			break;
		case E_L_BRACKET:
			row = 4;
			break;
		case E_R_BRACKET:
			row = 5;
			break;
		case E_ID:
			row = 6;
			break;
		case E_DOLLAR:
			row = 7;
			break;
		default:
			row = 99;
			break;
	}

	switch (input)
	{
		case E_PLUS:
		case E_MINUS:
		case E_CON:
			col = 0;
			break;
		case E_MUL:
		case E_DIV:
			col = 1;
			break;
		case E_LT:
		case E_LEQ:
		case E_GT:
		case E_GEQ:
			col = 2;
			break;
		case E_EQ:
		case E_NEQ:
			col = 3;
			break;
		case E_L_BRACKET:
			col = 4;
			break;
		case E_R_BRACKET:
			col = 5;
			break;
		case E_ID:
			col = 6;
			break;
		case E_DOLLAR:
			col = 7;
			break;
		default:
			col = 99;
			break;
	}

	if (row > 7 || col > 7) return X;
	else return precedence_table[row][col];
}

int test_rule (int count, expression_symbols one, expression_symbols two, expression_symbols three)
{
	switch (count)
	{
		case 1:
			if (one == E_ID)
			{
				// apply rule
			}
			break;
		case 3:
			if (one == E_L_BRACKET && two == E_NON_TERM && three == E_R_BRACKET)
			{
				// apply rule
			}
			else if (one == E_NON_TERM && three == E_NON_TERM)
			{
				switch (two)
				{
					case E_PLUS:
						// apply rule
					break;
					case E_MINUS:
						// apply rule
					break;
					case E_CON:
						// apply rule
					break;
					case E_MUL:
						// apply rule
					break;
					case E_DIV:
						// apply rule
					break;
					case E_LT:
						// apply rule
					break;
					case E_LEQ:
						// apply rule
					break;
					case E_GT:
						// apply rule
					break;
					case E_GEQ:
						// apply rule
					break;
					case E_EQ:
						// apply rule
					break;
					case E_NEQ:
						// apply rule
					break;
					default:
						//printf("NOT A RULE!\n");
						return 1;
				}
			}
			break;

		default:
			return 1;
			break;
	}
	stack_pop_times(count + 1);
	stack_push(E_NON_TERM);
	return 0;
}

int reduce (int count)
{
	switch (count)
	{
		case 1:
			if (test_rule(1, stack_peek_1(), X, X)) return 1; // Error in test_rule function
			break;
		case 3:
			if (test_rule(3, stack_peek_1(), stack_peek_2(), stack_peek_3())) return 1; // Error in test_rule function
			break;
		default:
			//printf("COUNT IS NOT 1 OR 3\n");
			return 1;
			break;
	}
	return 0;
}

int expression_parse (struct lexeme start_token, struct lexeme first_token)
{
	stack_init();
	struct lexeme current_token;
	expression_symbols current_token_symbol;
	expression_symbols stack_top_term;

	// Where to end
	lex_types end_token_type;

	if (start_token.type == L_PAR) end_token_type = R_PAR;          // ( ... )
	else end_token_type = SEMICOLON;																// return ... ; NEBO = ... ;

	/*if (end_token_type == SEMICOLON) printf("END TOKEN: SEMICOLON\n");
	else if (end_token_type == R_PAR) printf("END TOKEN: R_PAR\n");
	else printf("END TOKEN ERROR\n");*/

	stack_push(E_DOLLAR);

	if (start_token.type != first_token.type) {
		current_token = first_token; // if current_token == ; or ) exit
	} else {
		current_token = getToken();
	}

	if (current_token.type == end_token_type) return COMP_OK;

	do
	{

		// Get a symbol equivalent to current token
		current_token_symbol = token_to_symbol(current_token);
		/*if (current_token_symbol == E_ID) printf("CURRENT TOKEN SYMBOL: E_ID\n");
		else if (current_token_symbol == E_PLUS) printf("CURRENT TOKEN SYMBOL: E_PLUS\n");
		else if (current_token_symbol == E_MUL) printf("CURRENT TOKEN SYMBOL: E_MUL\n");
		else printf("CURRENT TOKEN SYMBOL ERROR\n");*/

		stack_top_term = stack_top_terminal();
		/*if (stack_top_term == E_ID) printf("STACK TOP TERMINAL: E_ID\n");
		else if (stack_top_term == E_PLUS) printf("STACK TOP TERMINAL: E_PLUS\n");
		else if (stack_top_term == E_MUL) printf("STACK TOP TERMINAL: E_MUL\n");
		else if (stack_top_term == E_DOLLAR) printf("STACK TOP TERMINAL: E_DOLLAR\n");
		else printf("STACK TOP TERMINAL ERROR\n");*/

		int count;

		switch(get_op(stack_top_term, current_token_symbol))
		{
			case E:
				//printf("CASE: E\n");
				stack_push(current_token_symbol);
				current_token = getToken();
				break;
			case S:
				//printf("CASE: S\n");
				stack_push_after_top_terminal(S);
				stack_push(current_token_symbol);
				current_token = getToken();
				break;
			case R:
				//printf("CASE: R\n");
				count = stack_until_shift();
				if (reduce(count)) return COMP_ERR_SA; // Error in reduce function
				break;
			case X:
				//printf("CASE: X\n");
				return COMP_ERR_SA;
				break;
			default:
				//printf("CASE ERROR\n");
				return COMP_ERR_SA;
				break;
		}
		//stack_print();

	} while (!(stack_top_terminal() == E_DOLLAR && current_token.type == end_token_type));

	//printf("EXPRESSION PARSING OK\n");
	return COMP_OK;
}