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

int test_rule (int count, stack_item one, stack_item two, stack_item three)
{
	switch (count)
	{
		case 1:
			if (one.symbol == E_ID)
			{
				return E_TO_I;
			}
			break;
		case 3:
			if (three.symbol == E_L_BRACKET && two.symbol == E_NON_TERM && one.symbol == E_R_BRACKET)
			{
				return LBRA_E_RBRA;
			}
			else if (one.symbol == E_NON_TERM && three.symbol == E_NON_TERM)
			{
				switch (two.symbol)
				{
					case E_PLUS:
						return E_PLUS_E;
						break;
					case E_MINUS:
						return E_MINUS_E;
						break;
					case E_CON:
						return E_CON_E;
						break;
					case E_MUL:
						return E_MUL_E;
						break;
					case E_DIV:
						return E_DIV_E;
						break;
					case E_LT:
						return E_LT_E;
						break;
					case E_LEQ:
						return E_LEQ_E;
						break;
					case E_GT:
						return E_GT_E;
						break;
					case E_GEQ:
						return E_GEQ_E;
						break;
					case E_EQ:
						return E_EQ_E;
						break;
					case E_NEQ:
						return E_NEQ_E;
						break;
					default:
						//printf("NOT A RULE!\n");
						return NOT_A_RULE;
						break;
				}
			}
			break;

		default:
			return NOT_A_RULE;
			break;
	}
	return NOT_A_RULE;
}

int reduce (int count)
{
	stack_item tmp = {0};
	struct lexeme empty_token = {0};

	rules rule;
	lex_types non_term_type;
	comp_err semantics_check;

	if (count == 1) {

		rule = test_rule(1, stack_peek_1(), tmp, tmp);
		semantics_check = test_semantics(rule, stack_peek_1(), tmp, tmp, &non_term_type);
	
	} else if (count == 3) {
	
		rule = test_rule(3, stack_peek_1(), stack_peek_2(), stack_peek_3());
		semantics_check = test_semantics(rule, stack_peek_1(), stack_peek_2(), stack_peek_3(), &non_term_type);
	
	} else {
			//printf("COUNT IS NOT 1 OR 3\n");
			return COMP_ERR_SA;
	}

	if (rule == NOT_A_RULE) return COMP_ERR_SA;
	if (semantics_check != COMP_OK) return semantics_check;


	stack_pop_times(count + 1);
	stack_push(E_NON_TERM, non_term_type, empty_token);
	return COMP_OK;
}

int test_semantics (rules rule, stack_item one, stack_item two, stack_item three, lex_types *non_term_type)
{
	// Just to get rid of warnings
	stack_item item = two;
	two = item;

	//printf("rule: %d\n", rule);
	//printf("one.type: %d\n", one.type);
	//printf("two.type: %d\n", two.type);
  //printf("three.type: %d\n", three.type);

	switch (rule)
	{
		case E_TO_I:      // E -> i

			// if (one is undefined)
			// return COMP_ERR_UNDEF_VAR;
			*non_term_type = one.type;
			break;
		case LBRA_E_RBRA: // E -> (E)

			// if (two is undefined)
			// return COMP_ERR_UNDEF_VAR;
			*non_term_type = two.type;
			break;
		case E_PLUS_E:    // E -> E + E
		case E_MINUS_E:   // E -> E - E
		case E_MUL_E:     // E -> E * E

			// if (one is undefined or three is undefined)
			// return COMP_ERR_UNDEF_VAR;

			if (one.type == STR_LIT || three.type == STR_LIT) {
				return COMP_ERR_MISMATCHED_TYPES;
			}

			if (one.type == DECIMAL_LIT || three.type == DECIMAL_LIT) {
				*non_term_type = DECIMAL_LIT;
			} else {
				*non_term_type = INT_LIT;
			}

			break;
		case E_CON_E:     // E -> E . E

			// if (one is undefined or three is undefined)
			// return COMP_ERR_UNDEF_VAR;

			if (one.type == INT_LIT || one.type == DECIMAL_LIT || three.type == INT_LIT || three.type == DECIMAL_LIT)
			{
				return COMP_ERR_MISMATCHED_TYPES;
			} else {
				*non_term_type = STR_LIT;
			}

			break;
		case E_DIV_E:     // E -> E / E

			// if (one is undefined or three is undefined)
			// return COMP_ERR_UNDEF_VAR;

			if (one.type == STR_LIT || three.type == STR_LIT) {
				return COMP_ERR_MISMATCHED_TYPES;
			}

			*non_term_type = DECIMAL_LIT;

			break;
		case E_LT_E:      // E -> E < E
		case E_GT_E:      // E -> E > E
		case E_LEQ_E:     // E -> E <= E
		case E_GEQ_E:     // E -> E >= E
		case E_EQ_E:      // E -> E === E
		case E_NEQ_E:     // E -> E !== E

			*non_term_type = INT_LIT;
			break;
		default:
			break;
	}
	return COMP_OK;
}

int expression_parse (struct lexeme start_token, struct lexeme first_token)
{
	stack_init();
	struct lexeme empty_token = {0};
	struct lexeme current_token;
	stack_item stack_current_token;
	stack_item stack_top_term;
	comp_err reduce_error;

	// Where to end
	lex_types end_token_type;

	if (start_token.type == L_PAR) end_token_type = R_PAR;          // ( ... )
	else end_token_type = SEMICOLON;																// return ... ; NEBO = ... ;

	/*if (end_token_type == SEMICOLON) printf("END TOKEN: SEMICOLON\n");
	else if (end_token_type == R_PAR) printf("END TOKEN: R_PAR\n");
	else printf("END TOKEN ERROR\n");*/

	stack_push(E_DOLLAR, 0, empty_token);

	if (start_token.type != first_token.type) {
		current_token = first_token;
	} else {
		current_token = getToken();
	}

	if (current_token.type == end_token_type) return COMP_OK;

	do
	{

		// Get a symbol equivalent to current token
		stack_current_token.symbol = token_to_symbol(current_token);
		stack_current_token.type = current_token.type;
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

		switch(get_op(stack_top_term.symbol, stack_current_token.symbol))
		{
			case E:
				//printf("CASE: E\n");
				stack_push(stack_current_token.symbol, stack_current_token.type, empty_token);
				current_token = getToken();
				break;
			case S:
				//printf("CASE: S\n");
				stack_push_after_top_terminal(S, 0, empty_token);
				stack_push(stack_current_token.symbol, stack_current_token.type, empty_token);
				current_token = getToken();
				break;
			case R:
				//printf("CASE: R\n");
				count = stack_until_shift();
				reduce_error = reduce(count);
				if (reduce_error != COMP_OK) return reduce_error; // Error in reduce function
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

	} while (!(stack_top_terminal().symbol == E_DOLLAR && current_token.type == end_token_type));

	//printf("EXPRESSION PARSING OK\n");
	return COMP_OK;
}