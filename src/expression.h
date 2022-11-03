#ifndef EXPRESSION_H
#define EXPRESSION_H

typedef enum {

	// Expression symbols
	PLUS,   		// +
	MINUS,  		// -
	CON,   		  // .
	MUL,    		// *
	DIV,    		// /
	LT,     		// <
	GT,     		// >
	LEQ,    		// <=
	GEQ,    		// >=
	EQ,     		// ===
	NEQ,    		// !==
	L_BRACKET, 	// (
	R_BRACKET, 	// )
	ID,        	// i
	DOLLAR,    	// $

	// Precedence table actions
	R, // >
	S, // <
	E, // =
	X, // error
} expression_symbols;

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

#endif