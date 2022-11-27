#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"

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

	int function_call_cnt;		/**< function call id */

	int inFuntion;
	char *function_name;		/**< function call/definition name */
	int param_count;			/**< function call/definition parameter count */
	struct params params[10];	/**< function call parameters data */

	char *param_names[100];	/**< names of function definition parameters */
	type param_types[100];	/**< types of function definition parameters */
};


void generatorInit();


//need to know how many params we have, what types are there, what is the return value
void generatorPrepare(struct bs_data *data);

char* generatorCalculateId();

void generatorPushParamStringNonB(char *str);

int generatorPushParam();

void generatorFunWrite();

void generatorBuiltinFunctions();

void  generatorFunWrite();

int  generatorFunReadf();

int  generatorFunReadi();

int  generatorFunReads();

int generatorFunStrLen();

void generatorExecute();

char* convertString();

void convertCharToEsc(char character, char* converted, int* position);

int realloc_global_str(const char *str);

int realloc_local_str(const char *str);

int realloc_local_str_var(const char *str);

int realloc_function_def_str(const char *str);

int defvar_global(const char *var_name);

void generatorWriteCode();

int generatorAssigment(const char* var_name, int isGlobal);
//test
int generatorFunWriteR();

int generatorFunOrd();

int generatorFunChr();

int generatorFunSubstring();

int generateBuiltInFunc(char* funName);
//for this maybe use new file string .h / string.c

int generator_finish();

void generator_reset();
