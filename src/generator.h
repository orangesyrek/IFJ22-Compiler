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

	int function_call_cnt;

	char *function_name;
	int param_count;
	struct params params[10];
	char **param_names;
	char **param_types;
};


void generatorInit();


//need to know how many params we have, what types are there, what is the return value
void generatorPrepare(struct bs_data *data);

char* generatorCalculateId();

void generatorPushParamStringNonB(char *str);

void generatorPushParamString(char *str);

void generatorFunWrite();

void generatorBuiltinFunctions();

void  generatorFunWrite();

void  generatorFunReadf();

void  generatorFunReadi();

void  generatorFunReads();

void generatorFunStrLen();

void generatorExecute();



//for this maybe use new file string .h / string.c
