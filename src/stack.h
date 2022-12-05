/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: stack.h
 * Author(s): xpauli08
 */

#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 100

#include "expression.h"

/**
 * @brief      Intialize the stack
 */
void 
stack_init();

/**
 * @brief      Push a symbol onto the stack
 *
 * @param[in]  symbol  The symbol to be pushed
 * @param[in]  type    Token type
 * @param[in]  token   The token
 *
 * @return     return 1 if stack is full, else 0
 */
int
stack_push(expression_symbols symbol, lex_types type, struct lexeme token);

/**
 * @brief      Insert a symbol after the top terminal
 *
 * @param[in]  symbol  The symbol
 * @param[in]  type    The type
 * @param[in]  token   The token
 *
 * @return     return 1 if stack is full, else 0
 */
int
stack_push_after_top_terminal (expression_symbols symbol, lex_types type, struct lexeme token);

/**
 * @brief      Pop a symbol from the top of the stack
 */
void
stack_pop();

/**
 * @brief      Pop multiple symbols from the top of the stack
 *
 * @param[in]  times  How many symbols to be popped
 */
void
stack_pop_times(int times);

/**
 * @brief      Peek at the topmost item on the stack
 *
 * @return     The topmost stack item
 */
stack_item
stack_peek_1();

/**
 * @brief      Peek at the 2nd topmost item on the stack
 *
 * @return     The 2nd topmost stack item
 */
stack_item
stack_peek_2();

/**
 * @brief      Peek at the 3rd topmost item on the stack
 *
 * @return     The 3rd topmost stack item
 */
stack_item
stack_peek_3();

/**
 * @brief      Peek at any stack item
 *
 * @param[in]  pos   The position of the item
 *
 * @return     The stack item with position pos
 */
stack_item
stack_get(int pos);

/**
 * @brief      Peek at the topmost terminal of the stack
 *
 * @return     The stack topmost terminal
 */
stack_item
stack_top_terminal();

/**
 * @brief      Gets the position of the topmost terminal on the stack
 *
 * @return     Topmost terminal position, -1 if none was found
 */
int
stack_top_terminal_pos();

/**
 * @brief      Count the number of items on the stack before shift symbol
 *
 * @return     Number of items on the stack before shift symbol, -1 if there is no stop symbol
 */
int
stack_until_shift();

/**
 * @brief      Check if stack is full
 *
 * @return     1 if stack is full, else 0
 */
int
stack_is_full();

/**
 * @brief      Check if stack is empty
 *
 * @return     1 if stack is empty, else 0
 */
int
stack_is_empty();

/**
 * @brief      Prints the stack the same way as in presentations, for debugging purposes
 */
void
stack_print();

#endif
