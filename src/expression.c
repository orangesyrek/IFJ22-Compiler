#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "expression.h"
#include "symtab.h"
#include "generator.h"

extern int top;
extern expression_symbols stack[MAX_STACK_SIZE];

extern struct compiler_ctx *ctx;
extern struct generator generator;

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

expression_symbols
token_to_symbol (struct lexeme token)
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

expression_symbols
get_op (expression_symbols stack_top, expression_symbols input)
{
	int row;
	int col;

	// Gets the precedence table row based on terminal on the top of the stack
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

	// Gets the precedence table column based on terminal on the input 
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

rules
test_rule (int count, stack_item one, stack_item two, stack_item three)
{
	switch (count)
	{
		// count == 1 => this rule only needs one terminal
		case 1:
			if (one.symbol == E_ID)
			{
				return E_TO_I;
			}
			break;

		// count == 3 => this rule needs three terminals
		case 3:
			// E -> (E)
			if (three.symbol == E_L_BRACKET && two.symbol == E_NON_TERM && one.symbol == E_R_BRACKET)
			{
				return LBRA_E_RBRA;
			}
			else if (one.symbol == E_NON_TERM && three.symbol == E_NON_TERM)
			{
				switch (two.symbol)
				{
					// E -> E + E
					case E_PLUS:
						return E_PLUS_E;
						break;

					// E -> E - E
					case E_MINUS:
						return E_MINUS_E;
						break;
					
					// E -> E . E
					case E_CON:
						return E_CON_E;
						break;
					
					// E -> E * E
					case E_MUL:
						return E_MUL_E;
						break;
					
					// E -> E / E
					case E_DIV:
						return E_DIV_E;
						break;
					
					// E -> E < E
					case E_LT:
						return E_LT_E;
						break;
					
					// E -> E <= E
					case E_LEQ:
						return E_LEQ_E;
						break;
					
					// E -> E > E
					case E_GT:
						return E_GT_E;
						break;
					
					// E -> E >= E
					case E_GEQ:
						return E_GEQ_E;
						break;
					
					// E -> E === E
					case E_EQ:
						return E_EQ_E;
						break;
					
					// E -> E !== E
					case E_NEQ:
						return E_NEQ_E;
						break;
					
					// Rule not found
					default:
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

comp_err
reduce (int count)
{
	// Temporary empty stack_item, set to {0}, just so I don't pass it into the function unset
	stack_item tmp = {0};
	// Temporary empty token, set to {0}, to be passed into stack_push function with non terminal (they don't correspond to any token)
	struct lexeme empty_token = {0};

	rules rule; // Rule to be returned
	lex_types non_term_type; // Type of the non terminal after the rule reduction has occured
	comp_err semantics_check;

	// Based on count, test if the rule exists check the semantics
	if (count == 1) {

		rule = test_rule(1, stack_peek_1(), tmp, tmp);
		semantics_check = test_semantics(rule, stack_peek_1(), tmp, tmp, &non_term_type);

	} else if (count == 3) {

		rule = test_rule(3, stack_peek_1(), stack_peek_2(), stack_peek_3());
		semantics_check = test_semantics(rule, stack_peek_1(), stack_peek_2(), stack_peek_3(), &non_term_type);

	} else {
			return COMP_ERR_SA;
	}

	// Return errors if something is wrong
	if (rule == NOT_A_RULE) return COMP_ERR_SA;
	if (semantics_check != COMP_OK) return semantics_check;

	// Finally, reduce
	stack_pop_times(count + 1);
	stack_push(E_NON_TERM, non_term_type, empty_token);
	return COMP_OK;
}

comp_err
test_semantics (rules rule, stack_item one, stack_item two, stack_item three, lex_types *non_term_type)
{
	// This switch is for testing semantics (can't check variable semantics, but can check constant semantics)
  switch (rule)
	{
		// E -> i
		case E_TO_I:

			// Check if variable is in the table of symbols (is defined)
			// Only need to test here because this rule will always be applied before the others
			if (one.type == VAR || one.type == FUN_ID) {
				if (ctx->in_function) {
					if (!symtabSearch(ctx->local_sym_tab, one.token.id)) {
						return COMP_ERR_UNDEF_VAR;
					}
				} else {
					if (!symtabSearch(ctx->global_sym_tab, one.token.id)) {
						return COMP_ERR_UNDEF_VAR;
					}
				}
			}

			*non_term_type = one.type;
			break;

 		// E -> (E)
		case LBRA_E_RBRA:

			*non_term_type = two.type;
			break;

		// E -> E + E
		case E_PLUS_E:    

			if (one.type == STR_LIT || three.type == STR_LIT) {
				return COMP_ERR_MISMATCHED_TYPES;
			}

			if (one.type == DECIMAL_LIT || three.type == DECIMAL_LIT) {
				*non_term_type = DECIMAL_LIT;
			} else {
				*non_term_type = INT_LIT;
			}
			break;

		// E -> E - E
		case E_MINUS_E:   

			if (one.type == STR_LIT || three.type == STR_LIT) {
				return COMP_ERR_MISMATCHED_TYPES;
			}

			if (one.type == DECIMAL_LIT || three.type == DECIMAL_LIT) {
				*non_term_type = DECIMAL_LIT;
			} else {
				*non_term_type = INT_LIT;
			}
			break;

		// E -> E * E
		case E_MUL_E:     

			if (one.type == STR_LIT || three.type == STR_LIT) {
				return COMP_ERR_MISMATCHED_TYPES;
			}

			if (one.type == DECIMAL_LIT || three.type == DECIMAL_LIT) {
				*non_term_type = DECIMAL_LIT;
			} else {
				*non_term_type = INT_LIT;
			}
			break;

		// E -> E . E
		case E_CON_E:     

			if (one.type == INT_LIT || one.type == DECIMAL_LIT || three.type == INT_LIT || three.type == DECIMAL_LIT)
			{
				return COMP_ERR_MISMATCHED_TYPES;
			} else {
				*non_term_type = STR_LIT;
			}
			break;

		// E -> E / E
		case E_DIV_E:     

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

		// Should not happen
		default:
			break;
	}

	// This switch will call the corresponding function for generating code
	switch (rule)
	{
		// E -> i
		case E_TO_I:      

			if (generatorExpression(one.token)) return COMP_ERR_INTERNAL;
			break;

		// E -> (E)
		case LBRA_E_RBRA: 
			break;

		// E -> E + E
		case E_PLUS_E:   
			generator.isIf = 0;
			generatorExprPlus();
			break;

		// E -> E - E
		case E_MINUS_E:   
			generator.isIf = 0;
			generatorExprMinus();
			break;

		// E -> E * E
		case E_MUL_E:     
			generator.isIf = 0;
			generatorExprMul();
			break;

		// E -> E . E
		case E_CON_E:     
			generator.isIf = 0;
			generatorExprConcat();
			break;

		// E -> E / E
		case E_DIV_E:     
			generator.isIf = 0;
			generatorExprDiv();
			break;

		// E -> E < E
		case E_LT_E:      
			generator.isIf = 1;
			generatorIfLess();
			break;

		// E -> E > E	
		case E_GT_E:      
			generator.isIf = 1;
			generatorIfGreater();
			break;

		// E -> E <= E
		case E_LEQ_E:     
			generator.isIf = 1;
			generatorIfEqualsLess();
			break;

		// E -> E >= E
		case E_GEQ_E:     
			generator.isIf = 1;
			generatorIfEqualsGreater();
			break;

		// E -> E === E
		case E_EQ_E:      
			generator.isIf = 1;
			generatorIfEquals();
			break;

		// E -> E !== E
		case E_NEQ_E:     
			generator.isIf = 1;
			generatorIfNotEquals();
			break;

		default:
			break;
	}
	return COMP_OK;
}

comp_err
expression_parse (struct lexeme start_token, struct lexeme first_token)
{
	stack_init();

	// Temporary empty token, set to {0}, to be passed into stack_push function with operation (they don't correspond to any token)
	struct lexeme empty_token = {0};
	struct lexeme current_token;
	stack_item stack_current_token; // Current token to be pushed onto stack
	stack_item stack_top_term; // Terminal on the top of the stack
	comp_err reduce_error;

	// Where to end
	lex_types end_token_type;

	if (start_token.type == L_PAR) end_token_type = R_PAR;          // ( ... )
	else end_token_type = SEMICOLON;																// return ... ; NEBO = ... ;

	// At the beginning, push a dollar onto stack
	stack_push(E_DOLLAR, 0, empty_token);

	// Determine which token to start with based on start_token and first_token
	if (start_token.type != first_token.type) {
		current_token = first_token;
	} else {
		current_token = getToken();
	}

	if (current_token.type == end_token_type) return COMP_OK;

	do
	{
		// Get the symbol and type of the current token
		stack_current_token.symbol = token_to_symbol(current_token);
		stack_current_token.type = current_token.type;

		// Get the top terminal on the stack
		stack_top_term = stack_top_terminal();

		int count;

		// Determine the operation that should be done
		switch(get_op(stack_top_term.symbol, stack_current_token.symbol))
		{
			// Equal
			case E:
				stack_push(stack_current_token.symbol, stack_current_token.type, empty_token);
				current_token = getToken();
				break;

			// Shift
			case S:
				stack_push_after_top_terminal(S, 0, empty_token);
				stack_push(stack_current_token.symbol, stack_current_token.type, current_token);
				current_token = getToken();
				break;

			// Reduce
			case R:
				count = stack_until_shift();
				reduce_error = reduce(count);
				if (reduce_error != COMP_OK) return reduce_error; // Error in reduce function
				break;

			// Wrong combination
			case X:
				return COMP_ERR_SA;
				break;

			default:
				return COMP_ERR_SA;
				break;
		}

	// If only DOLLAR is on the stack and we found the end token, this will stop
	} while (!(stack_top_terminal().symbol == E_DOLLAR && current_token.type == end_token_type));
	
	// Call the function generating the end of expression and return
	if (generatorExpressionCalculated()) return COMP_ERR_INTERNAL;
	return COMP_OK;
}