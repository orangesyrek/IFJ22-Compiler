/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: generator.h
 * Author(s): xvalik04, xjanot04
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"
#include "lexical_analysis.h"

/**
 * @brief Stores function call parameters information
 */
struct params {
	int is_constant;
	union {
		char *var_name;
		int int_val;
		double flt_val;
		char *str_val;
	} value;
	type type;
};


struct generator {
	char *global_str; /**< string that holds global variable and global code */
	char *global_str_var; /**< string that prevents of defvar duplicites*/
	char *local_str_var; /**< string that holds definitions local variables*/
	char *local_str; /**< string that holds defition of user created function */
	char *function_def_str; /**< string that holds function definitions*/
	char *runtimeCallStr; /**< string that holds definitions of built in function and conversions*/

	int function_call_cnt;		/**< function call id */
	int ifLabelCount; /** Label naming*/
	int ifCountMax; /** Maximal count of nested if's*/
	int whileLabelCount; /** Label naming*/
	int whileCountMax; /** Maximal count of nested while's*/

	int isIf;
	int isOneSizedExpr;
	int inFuntion;
	char *function_name;		/**< function call/definition name */
	int param_count;			/**< function call/definition parameter count */
	struct params params[100];	/**< function call parameters data */
	char *param_names[100];	/**< names of function definition parameters */
	type param_types[100];	/**< types of function definition parameters */
};




/**
Functions that work with aritmetic and string operators
and generating code to calculate expressions
 */
int generatorExpression(struct lexeme token);
int generatorExpressionCalculated();
int generatorExprPlus();
int generatorExprMinus();
int generatorExprMul();
int generatorExprDiv();
int generatorExprConcat();

/**
Functions that work with if conditions
and generating code to compare variables and values
 */

int generatorIfTrueEnd();
int generatorIfEquals();
int generatorIfFalse();
int generatorIfTrue();
int generatorIfEnd();
int generatorIfLess();
int generatorIfGreater();
int generatorIfEqualsGreater();
int generatorIfEqualsLess();
int generatorIfNotEquals();


/**
Functions that work with cycles
and generating code
 */

int generatorWhileStart();
int generatorWhileBody();
int generatorWhileEnd();


/**
Functions that generate type conversions

 */
 int plusMinMulConversion();
 int concatConversion();
 int divConversion();
 int equalsConversion();
 int exprConversion();

 /**
 Function that initialize'strings in generator
 prints header code
 prints global defvars

  */

void generatorInit();

/**
Function that generate calling function
based on name of function from struct generator
 */


int generatorCall();

/**
Function that pushes parametr based on data type
on data stack
 */

int generatorPushParam();

/**
Function that pushes parametr based on data type
on data stack
 */

 /**
Funtions that generates built in functions
  */

int floatval();
int intval();
int strval();
int  generatorFunReadf();
int  generatorFunReadi();
int  generatorFunReads();
int generatorFunStrLen();
int generatorFunWrite();
int generatorFunOrd();
int generatorFunChr();
int generatorFunSubstring();

/**
Function that generater call of function
 based on name in generator struct
 */

void generatorExecute();

/**
Functions that converts characters in string to backspace notations

 */

char* convertString(char* nonConvertedstr);
void convertCharToEsc(char character, char* converted, int* position);


/**
Functions that realocates strings in generator

 */

int realloc_global_str(const char *str);
int realloc_global_str_var(const char *str);
int realloc_local_str(const char *str);
int realloc_local_str_var(const char *str);
int realloc_function_def_str(const char *str);
int realloc_runtimeCallStr(const char *str);

/**
Function that generate definitions of global variable

 */


int defvar_global(const char *var_name);

/**
Function print strings in genetarator after we compile program

 */


void generatorWriteCode();


/**
Function that generate assignment

 */

int generatorAssigment();

/**
 * @brief Generate exit labels and jump to the end in the global scope.
 *
 * @return 0 on success, non-zero otherwise.
 */
int generator_finish();

/**
 * @brief Resets the generator structure parameter count.
*/
void generator_reset();

/**
 * @brief Generate function header.
 *
 * @return 0 on success, non-zero otherwise.
 */
int generate_function_def();

/**
 * @brief Concatenate function header and body.
 *
 * @return 0 on success, non-zero otherwise.
 */
int generator_function_def_end();

/**
 * @brief Generate functions return statement.
 *
 * @return 0 on success, non-zero otherwise.
 */
int generate_function_return();

/**
 * @brief Generate return type checking.
 *
 * @param[in] return_type Expected return type.
 * @return 0 on success, non-zero otherwise.
 */
int return_type_check(type return_type);

/**
 * @brief Generate a local variable.
 *
 * @param[in] var_name Variable name
 * @return 0 on success, non-zero otherwise.
 */
int defvar_local(const char *var_name);
