#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"
#include "lexical_analysis.h"

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
	char *global_str;
	char *local_str_var;
	char *local_str;
	char *function_def_str;
	char *runtimeCallStr;

	int function_call_cnt;		/**< function call id */
	int ifLabelCount; /** Label naming*/
	int ifCountMax; /** Maximal count of nested if's*/
	int whileLabelCount; /** Label naming*/
	int whileCountMax; /** Maximal count of nested while's*/

	int isIf;
	int inFuntion;
	char *function_name;		/**< function call/definition name */
	int param_count;			/**< function call/definition parameter count */
	struct params params[10];	/**< function call parameters data */
	char *param_names[100];	/**< names of function definition parameters */
	type param_types[100];	/**< types of function definition parameters */
};


int divConversion();


int generatorExpression(struct lexeme token);
int generatorExpressionCalculated();
int generatorExprPlus();
int generatorExprMinus();
int generatorExprMul();
int generatorExprDiv();
int generatorExprConcat();

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

int generatorWhileStart();
int generatorWhileBody();
int generatorWhileEnd();

int concatConversion();
int plusMinMulConversion();

void generatorInit();

int generatorCall();


//need to know how many params we have, what types are there, what is the return value
void generatorPrepare(struct bs_data *data);

char* generatorCalculateId();

void generatorPushParamStringNonB(char *str);

int generatorPushParam();

void generatorBuiltinFunctions();

int  generatorFunReadf();

int  generatorFunReadi();

int  generatorFunReads();

int generatorFunStrLen();

void generatorExecute();

char* convertString(char* nonConvertedstr);

void convertCharToEsc(char character, char* converted, int* position);

int realloc_global_str(const char *str);

int realloc_local_str(const char *str);

int realloc_local_str_var(const char *str);

int realloc_function_def_str(const char *str);

int realloc_runtimeCallStr(const char *str);

int defvar_global(const char *var_name);

void generatorWriteCode();

int generatorAssigment(const char* var_name);
//test
int generatorFunWrite();

int generatorFunOrd();

int generatorFunChr();

int generatorFunSubstring();

int generateBuiltInFunc(char* funName);
//for this maybe use new file string .h / string.c

int generator_finish();

void generator_reset();

int generate_function_def();

int generator_function_def_end();

int generate_function_return();

int return_type_check(type return_type);

int defvar_local(const char *var_name);