/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: syntactic_analysis.h
 * Author(s): xjanot04
 */

#ifndef SYNT_ANALYSIS_H
#define SYNT_ANALYSIS_H

#include "compiler.h"

/**
 * @brief Main parser function.
 *	Syntactically check the prolog and then the rest of the program.
 *
 * @return COMP_OK on success, error otherwise.
 */
comp_err synt_parse();

/**
 * @brief Check if there were any undefined functions referenced.
 *
 * @return 0 if there were not, 1 if there were any.
 */
int check_undefined_functions();

#endif
