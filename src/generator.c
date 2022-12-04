/*
 * FIT VUT 2022 - IFJ Project
 * Implementation of a compiler for an imperative language IFJ22
 *
 * File: generator.c
 * Author(s): xvalik04, xjanot04
 */

#define _GNU_SOURCE /* strdup */
#include "compiler.h"
#include "generator.h"

struct generator generator = {0};


/*
Todo
NEED TO SOLVE DEFVAR INSIDE WHILELOOP
typecasting other builtin functions


*/

unsigned long id = 0;

void generatorInit(){
  char* ptr;
  if (realloc_local_str("")) exit(COMP_ERR_INTERNAL);
  if (realloc_local_str_var("")) exit(COMP_ERR_INTERNAL);
  if (realloc_function_def_str("")) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str("")) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var("")) exit(COMP_ERR_INTERNAL);
  if (realloc_runtimeCallStr("")) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, ".IFJcode22\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@!bool\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@!tmp1\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@!tmp2\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@!type1\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@!type2\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@!ret\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str_var(ptr)) exit(COMP_ERR_INTERNAL);



  // if (generatorFunReads()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
  // if (generatorFunReadi()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
  // if (generatorFunReadf()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
  // if (generatorFunOrd()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
  // if (generatorFunChr()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
  // if (generatorFunStrLen()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
  // if (generatorFunSubstring()) {
  //   exit(COMP_ERR_INTERNAL);
  // }
}

void convertCharToEsc(char character, char* converted, int* position){
  switch(character){
    case 'n':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '1';
      converted[*position+3] = '0';
      *position+=4;
      break;
    case 'b':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '0';
      converted[*position+3] = '8';
      *position+=4;
      break;
    case 'f':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '1';
      converted[*position+3] = '2';
      *position+=4;
      break;
    case 'a':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '0';
      converted[*position+3] = '7';
      *position+=4;
      break;
    case 'v':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '1';
      converted[*position+3] = '1';
      *position+=4;
      break;
    case 't':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '0';
      converted[*position+3] = '9';
      *position+=4;
      break;
    case 'r':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '1';
      converted[*position+3] = '3';
      *position+=4;
      break;
    case '\\':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '9';
      converted[*position+3] = '2';
      *position+=4;
      break;
    case '\"':
      converted[*position] = '\\';
      converted[*position+1] = '0';
      converted[*position+2] = '3';
      converted[*position+3] = '4';
      *position+=4;
      break;
    default:
      converted[*position] = character;
      *position+=1;
      break;
  }
}

char* convertString(char* nonConvertedstr){
  char* converted;
  int index = 0;
  char character;
  int position = 0;

  converted = calloc(1, 512);
  if (!converted) {
    return NULL;
  }

  if(nonConvertedstr){
    character = nonConvertedstr[index];
    while(character != '\0'){
      if((character <= 32) || (character == 35)) {
        converted[position] = '\\';
        converted[position+1] = '0';
        converted[position+2] = (character / 10) + 48;
        converted[position+3] = (character % 10) + 48;
        position += 4;

      } else if (character == '\\') {
        character = nonConvertedstr[++index];
        convertCharToEsc(character, converted, &position);
      } else{
        converted[position] = character;
        position++;
      }
      character = nonConvertedstr[++index];
    }

  return converted;

  }else{
    return NULL;
  }
}



int defvar_global(const char *var_name){
  char *ptr;
  int ret = asprintf(&ptr, "DEFVAR GF@%s\n", var_name);
  if (ret == -1){
    return COMP_ERR_INTERNAL;
  }


  if (strstr(generator.global_str_var, ptr) == NULL){
    if (realloc_global_str_var(ptr)) return COMP_ERR_INTERNAL;
  }


  free(ptr);
  return 0;
}

int realloc_global_str(const char *str){

  if (!generator.global_str) {
    generator.global_str = strdup(str);
    if (!generator.global_str) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.global_str = realloc(generator.global_str, strlen(generator.global_str) + strlen(str) + 1);
    if (!generator.global_str) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.global_str, str);
  }

  return 0;
}

int realloc_global_str_var(const char *str){

  if (!generator.global_str_var) {
    generator.global_str_var = strdup(str);
    if (!generator.global_str_var) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.global_str_var = realloc(generator.global_str_var, strlen(generator.global_str_var) + strlen(str) + 1);
    if (!generator.global_str_var) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.global_str_var, str);
  }

  return 0;
}


int realloc_local_str_var(const char *str){

  if (!generator.local_str_var) {
    generator.local_str_var = strdup(str);
    if (!generator.local_str_var) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.local_str_var = realloc(generator.local_str_var, strlen(generator.local_str_var) + strlen(str) + 1);
    if (!generator.local_str_var) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.local_str_var, str);
  }

  return 0;
}

int realloc_local_str(const char *str){

  if (!generator.local_str) {
    generator.local_str = strdup(str);
    if (!generator.local_str) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.local_str = realloc(generator.local_str, strlen(generator.local_str) + strlen(str) + 1);
    if (!generator.local_str) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.local_str, str);
  }

  return 0;
}


int realloc_function_def_str(const char *str){

  if (!generator.function_def_str) {
    generator.function_def_str = strdup(str);
    if (!generator.function_def_str) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.function_def_str = realloc(generator.function_def_str, strlen(generator.function_def_str) + strlen(str) + 1);
    if (!generator.function_def_str) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.function_def_str, str);
  }

  return 0;
}

int realloc_runtimeCallStr(const char *str) {

  if (!generator.runtimeCallStr) {
    generator.runtimeCallStr = strdup(str);
    if (!generator.runtimeCallStr) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.runtimeCallStr = realloc(generator.runtimeCallStr, strlen(generator.runtimeCallStr) + strlen(str) + 1);
    if (!generator.runtimeCallStr) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.runtimeCallStr, str);
  }

  return 0;
}



int generatorAssigment(const char* var_name){
  char* ptr;
  //case globar var | local var
  //then need to solve typecasting
  if(generator.inFuntion){
    //printf("MOVE GF@%s GF@!ret\n", var_name);
    if (asprintf(&ptr, "MOVE LF@%s GF@!ret\n", var_name) == -1) return COMP_ERR_INTERNAL;
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  }else{
    if (asprintf(&ptr, "MOVE GF@%s GF@!ret\n", var_name) == -1) return COMP_ERR_INTERNAL;
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    //printf("MOVE LF@%s GF@!ret\n", var_name);
  }
  return 0;

}

//for non-builtin functions creating frame id etc
char* generatorCalculateId(){
char idstr[1024]; // need some dynamic string or strdup
sprintf(idstr, "%lu", id);
char *name = strcat("p", idstr); // id ++
return name;
}

//now only for strings, then need to rebuilt to take float, int, bool null
//only for non-builtin functions
void generatorPushParamStringNonB(char *str){
  char *name = generatorCalculateId();
  printf("DEFVAR TF@%s\n", name);
  printf("MOVE TF%s string@%s\n", name, str);//need some fun to convert white spaces to ascii chars

}

//before pushing need to convert them to string
//pushing and popping will reverse the order of strings -- idea maybe use own stack to change order back
int generatorPushParam(){
  char* ptr;
  char* str;

  if(generator.inFuntion){
    for (int i = 0; i < generator.param_count; i++) {
      if (generator.params[i].type == INT) {
        if (asprintf(&ptr, "PUSHS int@%d\n", generator.params[i].value.int_val) == -1) return COMP_ERR_INTERNAL;
        if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == FLOAT) {
        if (asprintf(&ptr, "PUSHS float@%a\n", generator.params[i].value.flt_val) == -1) return COMP_ERR_INTERNAL;
        if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == STRING) {
        str = convertString(generator.params[i].value.str_val);
        if (!str) {
          return COMP_ERR_INTERNAL;
        }
        if (asprintf(&ptr, "PUSHS string@%s\n", str) == -1) return COMP_ERR_INTERNAL;
        if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == UNKNOWN) {
          if (asprintf(&ptr, "PUSHS LF@%s\n", generator.params[i].value.var_name) == -1) {
            return COMP_ERR_INTERNAL;
          }
          if (realloc_local_str(ptr)) {
           return COMP_ERR_INTERNAL;
          }
      } else if (generator.params[i].type == T_NULL) {
        if (asprintf(&ptr, "PUSHS nil@nil\n") == -1) {
            return COMP_ERR_INTERNAL;
        }
        if (realloc_local_str(ptr)) {
         return COMP_ERR_INTERNAL;
        }
      }
    }
  } else{
      for (int i = 0; i < generator.param_count; i++) {
        if (generator.params[i].type == INT) {
          if (asprintf(&ptr, "PUSHS int@%d\n", generator.params[i].value.int_val) == -1) return COMP_ERR_INTERNAL;
          if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
        } else if (generator.params[i].type == FLOAT) {
          if (asprintf(&ptr, "PUSHS float@%a\n", generator.params[i].value.flt_val) == -1) return COMP_ERR_INTERNAL;
          if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
        } else if (generator.params[i].type == STRING) {
          str = convertString(generator.params[i].value.str_val);
          if (!str) {
            return COMP_ERR_INTERNAL;
          }
          if (asprintf(&ptr, "PUSHS string@%s\n", str) == -1) return COMP_ERR_INTERNAL;
          if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
        } else if (generator.params[i].type == UNKNOWN) {
          if (asprintf(&ptr, "PUSHS GF@%s\n", generator.params[i].value.var_name) == -1) return COMP_ERR_INTERNAL;
          if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
        } else if (generator.params[i].type == T_NULL) {
          if (asprintf(&ptr, "PUSHS nil@nil\n") == -1) return COMP_ERR_INTERNAL;
          if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
        }
      }
  }
  return COMP_OK;
}

void generatorExecute(){ // need to know function name
  char* ptr;

  if(generatorPushParam()){
    exit(COMP_ERR_INTERNAL);
  }

  if (strcmp("write", generator.function_name)){

  if (asprintf(&ptr, "CALL %s\n", generator.function_name) == -1) exit(COMP_ERR_INTERNAL);
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) exit(COMP_ERR_INTERNAL);
  } else {
    if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);
  }
}

  if(generatorCall()){
      exit(COMP_ERR_INTERNAL);
  }

}


int generatorCall(){
  if (!strcmp("write", generator.function_name)) {
    if(generatorFunWrite()) return COMP_ERR_INTERNAL;
    return 0;
  }else if(!strcmp("reads", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL reads") == NULL){
      if (generatorFunReads()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }else if(!strcmp("readi", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL readi") == NULL){
      if (generatorFunReadi()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }else if(!strcmp("readf", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL readf") == NULL){
      if (generatorFunReadf()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }else if(!strcmp("ord", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL ord") == NULL){
      if (generatorFunOrd()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }else if(!strcmp("chr", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL chr") == NULL){
      if (generatorFunChr()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }else if(!strcmp("strlen", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL strlen") == NULL){
      if (generatorFunStrLen()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }else if(!strcmp("substring", generator.function_name)){
    if (strstr(generator.runtimeCallStr, "LABEL substring") == NULL){
      if (generatorFunSubstring()) return COMP_ERR_INTERNAL;
    }
    return 0;
  }

  return 0;
}


void generatorWriteCode(){
  printf("%s\n", generator.global_str_var);
  printf("%s\n", generator.global_str);
  printf("%s\n", generator.function_def_str);
}


int generatorExpression(struct lexeme token){
  char* ptr;



  if (generator.inFuntion) {
    if (token.type == INT_LIT) {
      if (asprintf(&ptr, "PUSHS int@%d\n", token.value.int_val) == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == STR_LIT) {
      //here fixing fix to converted string
      if (asprintf(&ptr, "PUSHS string@%s\n", convertString(token.value.str_val)) == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == DECIMAL_LIT) {
      if (asprintf(&ptr, "PUSHS float@%a\n", token.value.flt_val) == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == VAR) {
      // difference betweem global var and local
      if (asprintf(&ptr, "PUSHS LF@%s\n", token.id) == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == KEYWORD_NULL) {
      if (asprintf(&ptr, "PUSHS nil@nil\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }
  } else {
    if (token.type == INT_LIT) {
      if (asprintf(&ptr, "PUSHS int@%d\n", token.value.int_val) == -1) return COMP_ERR_INTERNAL;
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

    }else if (token.type == STR_LIT) {
      //here fixing fix to converted string
      if (asprintf(&ptr, "PUSHS string@%s\n", convertString(token.value.str_val)) == -1) return COMP_ERR_INTERNAL;
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

    }else if (token.type == DECIMAL_LIT) {
      if (asprintf(&ptr, "PUSHS float@%a\n", token.value.flt_val) == -1) return COMP_ERR_INTERNAL;
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;


    }else if (token.type == VAR) {
      // difference betweem global var and local
      if (asprintf(&ptr, "PUSHS GF@%s\n", token.id) == -1) return COMP_ERR_INTERNAL;
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == KEYWORD_NULL) {
      if (asprintf(&ptr, "PUSHS nil@nil\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

    }
  }

  return 0;
}

int exprConversion(){

  char* ptr;



  if (asprintf(&ptr, "LABEL $exprConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFNEQ $exprConNotString GF@!type1 string@string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMPIFNEQ $exprConStringEmpty GF@!tmp1 string@\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@false\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "LABEL $exprConStringEmpty\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMPIFNEQ $exprConStringTrue GF@!tmp1 string@0\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@false\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "LABEL $exprConStringTrue\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@true\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $exprConNotString\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
//it can be int / float / null

  if (asprintf(&ptr, "JUMPIFNEQ $exprConNotInt GF@!type1 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "JUMPIFNEQ $exprConIntTrue GF@!tmp1 int@0\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@false\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "LABEL $exprConIntTrue\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@true\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $exprConNotInt\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFNEQ $exprConNotFloat GF@!type1 string@float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMPIFNEQ $exprConFloatTrue GF@!tmp1 float@0x0p+0\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@false\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "LABEL $exprConFloatTrue\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@true\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMP $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $exprConNotFloat\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
  //it can be null / bool

    if (asprintf(&ptr, "JUMPIFNEQ $exprConEnd GF@!type1 string@nil\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!bool bool@false\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $exprConEnd\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "return\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorExprPlus(){
  char* ptr;


  if (strstr(generator.runtimeCallStr, "$plusMinMulConversion") == NULL){
    if (plusMinMulConversion()) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $plusMinMulConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "ADDS\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }
  return 0;
}

int plusMinMulConversion(){
  char* ptr;

  if (asprintf(&ptr, "LABEL $plusMinMulConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "TYPE GF@!type2 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;




  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@nil\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type2 string@string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type2 string@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type2 string@nil\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

//error when string bool null come
// if not check for int


  if (asprintf(&ptr, "JUMPIFEQ $firstInt GF@!type1 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
//when we are here firt is float we dont know second

  if (asprintf(&ptr, "JUMPIFEQ $bothConverted GF@!type2 string@float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "INT2FLOAT GF@!tmp2 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMP $bothConverted\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
// done when first is float

  if (asprintf(&ptr, "LABEL $firstInt\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $bothConverted GF@!type2 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
//if second is float and first is int we just convert first to float

  if (asprintf(&ptr, "INT2FLOAT GF@!tmp1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $bothConverted\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;



  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorExprMinus(){
  char* ptr;

  if (strstr(generator.runtimeCallStr, "$plusMinMulConversion") == NULL){
    if (plusMinMulConversion()) return COMP_ERR_INTERNAL;
  }

    if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;

    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;

    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "CALL $plusMinMulConversion\n") == -1) return COMP_ERR_INTERNAL;

    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;

    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }


    if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }



  if (asprintf(&ptr, "SUBS\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }
  return 0;
}

int generatorExprMul(){
  char* ptr;

  if (strstr(generator.runtimeCallStr, "$plusMinMulConversion") == NULL){
    if (plusMinMulConversion()) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $plusMinMulConversion\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "MULS\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }
  return 0;
}

int generatorExprDiv(){
  char* ptr;
  // todo pop from stack typecast and then push back

//here should alocate local str too

  //calling function that generates div convertions
  if (strstr(generator.runtimeCallStr, "$divConversion") == NULL){
    if (divConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $divConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }



  if (asprintf(&ptr, "DIVS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }
  return 0;
}

int divConversion(){
  char *ptr;


/*
string, bool =>  error
int, null ==> typecast
float ==> do nothing
*/

  if (asprintf(&ptr, "LABEL $divConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  //check the type
  if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


// if its string bool -> error
  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
// if its float -> we are done with first
  if (asprintf(&ptr, "JUMPIFEQ $firstConvertedDiv GF@!type1 string@float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  //if its not int jump to not converted by int
  if (asprintf(&ptr, "JUMPIFNEQ $notFirstConversionDivInt GF@!type1 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  //if its int we are converting int to float
  if (asprintf(&ptr, "INT2FLOAT GF@!tmp1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  // we converted jump first converted
  if (asprintf(&ptr, "JUMP $firstConvertedDiv\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $notFirstConversionDivInt\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
  //if its not anything above it was null convert to 0.0

  if (asprintf(&ptr, "MOVE GF@!tmp1 float@0x0p+0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $firstConvertedDiv\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
// if its float -> we are done with first
  if (asprintf(&ptr, "JUMPIFEQ $secondConvertedDiv GF@!type1 string@float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;



  if (asprintf(&ptr, "JUMPIFNEQ $notSecondConversionDivInt GF@!type1 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "INT2FLOAT GF@!tmp2 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMP $secondConvertedDiv\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $notSecondConversionDivInt\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "MOVE GF@!tmp2 float@0x0p+0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $secondConvertedDiv\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  return 0;
}

int generatorExprConcat(){
  char* ptr;



  if (strstr(generator.runtimeCallStr, "$concatConversion") == NULL){
    if (concatConversion()) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "CALL $concatConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "CONCAT GF@!ret GF@!tmp1 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!ret\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
     if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
   } else {
     if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
   }
  return 0;
}

int concatConversion(){

  char* ptr;
  if (asprintf(&ptr, "LABEL $concatConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  //check the type
  if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


// if its string bool -> error
  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "JUMPIFNEQ $firstConvertedCon GF@!type1 string@nil\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "MOVE GF@!tmp1 string@ \n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $firstConvertedCon\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $!exit7 GF@!type1 string@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFNEQ $secondConvertedCon GF@!type1 string@nil\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "MOVE GF@!tmp2 string@ \n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $secondConvertedCon\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorExpressionCalculated(){
  char* ptr;
  if(!generator.isIf){
    if (asprintf(&ptr, "POPS GF@!ret\n") == -1) return COMP_ERR_INTERNAL;
    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }

  }else{
    if(generator.isOneSizedExpr){
      if (strstr(generator.runtimeCallStr, "$exprConversion") == NULL){
        if(exprConversion()) return COMP_ERR_INTERNAL;
      }
      if (asprintf(&ptr, "CALL $exprConversion\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "PUSHS int@0\n") == -1) return COMP_ERR_INTERNAL;
    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "POPS GF@!ret\n") == -1) return COMP_ERR_INTERNAL;
    if (generator.inFuntion) {
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    } else {
      if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    }

  }
  return 0;
}

int generatorIfEquals(){

  char* ptr;

  if (strstr(generator.runtimeCallStr, "$equalsConversion") == NULL){
    if (equalsConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $equalsConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "EQS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!bool\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}

int equalsConversion(){

    char* ptr;
    if (asprintf(&ptr, "LABEL $equalsConversion\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "TYPE GF@!type2 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "JUMPIFEQ $equalsConEnd GF@!type1 GF@!type2\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "MOVE GF@!tmp1 int@0\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "MOVE GF@!tmp2 int@1\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;



    if (asprintf(&ptr, "LABEL $equalsConEnd\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


    if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}


int generatorIfNotEquals(){

  char* ptr;
  if (strstr(generator.runtimeCallStr, "$equalsConversion") == NULL){
    if (equalsConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $equalsConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "EQS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "NOTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "POPS GF@!bool\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}

int relationConversion(){

      char* ptr;
      if (asprintf(&ptr, "LABEL $relationConversion\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "TYPE GF@!type1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "TYPE GF@!type2 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "JUMPIFEQ $relSkipNil GF@!type1 string@nil\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "JUMPIFEQ $relConEnd GF@!type1 GF@!type2\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;



      if (asprintf(&ptr, "LABEL $relSkipNil\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      //here if they are not with same type
      /*
      relCon0 sametype x sametype -> good
      relCon1 NULL x string -> null => ""
      relCon2
      */

      //null x string
      if (asprintf(&ptr, "JUMPIFNEQ $relCon2 GF@!type1 string@nil\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "JUMPIFNEQ $relcon3 GF@!type2 string@string\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
      //relCon1

      if (asprintf(&ptr, "MOVE GF@!tmp1 string@\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      //first is not nil
      if (asprintf(&ptr, "LABEL $relCon2\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      //it can be {string-nebude testovano}  number {bool - neni na foru zmineno}
      // takze z odpovedi na foru to muze byt jenom cislo => prevod na float u obou

      if (asprintf(&ptr, "JUMPIFEQ $relConFirstFloat GF@!type1 string@float\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "INT2FLOAT GF@!tmp1 GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "LABEL $relConFirstFloat\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "JUMPIFEQ $relConSecondFloat GF@!type2 string@float\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "INT2FLOAT GF@!tmp2 GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "LABEL $relConSecondFloat\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      //here are only cases null x any -> null => false ; any => bool via table
      if (asprintf(&ptr, "LABEL $relcon3\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "MOVE GF@!tmp1 bool@false\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "JUMPIFNEQ $relConSecNotNil GF@!type2 string@nil\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

      if (asprintf(&ptr, "MOVE GF@!tmp2 bool@false\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      //second not nill
      if (asprintf(&ptr, "LABEL $relConSecNotNil \n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


            if (asprintf(&ptr, "JUMPIFNEQ $relConSecNotInt GF@!type2 string@int\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMPIFNEQ $relConSecIntTrue GF@!tmp2 int@0\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "MOVE GF@!tmp2 bool@false\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "LABEL $relConSecIntTrue\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "MOVE GF@!tmp2 bool@true\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;




      if (asprintf(&ptr, "LABEL $relConSecNotInt \n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMPIFNEQ $relConSecNotFloat GF@!type2 string@float\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMPIFNEQ $relConSecFloatTrue GF@!tmp2 float@0x0p+0\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "MOVE GF@!tmp2 bool@false\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "LABEL $relConSecFloatTrue\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "MOVE GF@!tmp2 bool@true\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "LABEL $relConSecNotFloat \n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
//it can be only bool / string
            if (asprintf(&ptr, "JUMPIFNEQ $relConEnd GF@!type2 string@string\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


            if (asprintf(&ptr, "JUMPIFNEQ $relConSecStringTrue GF@!tmp2 string@\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "MOVE GF@!tmp2 bool@false\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "JUMP $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "LABEL $relConSecStringTrue\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

            if (asprintf(&ptr, "MOVE GF@!tmp2 bool@true\n") == -1) return COMP_ERR_INTERNAL;
            if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;



      if (asprintf(&ptr, "LABEL $relConEnd\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


      if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
      if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

    return 0;
}

int generatorIfLess(){

  char* ptr;

  if (strstr(generator.runtimeCallStr, "$relationConversion") == NULL){
    if (relationConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $relationConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "LTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!bool\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}

int generatorIfGreater(){

  char* ptr;

  if (strstr(generator.runtimeCallStr, "$relationConversion") == NULL){
    if (relationConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $relationConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "GTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!bool\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}

int generatorIfEqualsGreater(){

  char* ptr;

  if (strstr(generator.runtimeCallStr, "$relationConversion") == NULL){
    if (relationConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $relationConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "LTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "NOTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!bool\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;

}

int generatorIfEqualsLess(){
  char* ptr;


  if (strstr(generator.runtimeCallStr, "$relationConversion") == NULL){
    if (relationConversion()) return COMP_ERR_INTERNAL;

  }

  if (asprintf(&ptr, "POPS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "POPS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CALL $relationConversion\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHS GF@!tmp1\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "PUSHS GF@!tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "GTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "NOTS\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }


  if (asprintf(&ptr, "POPS GF@!bool\n") == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;

}


int generatorIfTrue(){
  if(generator.ifCountMax != 0){
    generator.ifLabelCount = generator.ifCountMax;
  }
  char* ptr;
  if (asprintf(&ptr, "LABEL trueif%d\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "JUMPIFEQ falseif%d GF@!bool bool@false\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  generator.ifLabelCount++;
  if(generator.ifLabelCount > generator.ifCountMax){
    generator.ifCountMax = generator.ifLabelCount;
  }

  return 0;
}

int generatorIfTrueEnd(){

  char* ptr;
  if (asprintf(&ptr, "JUMP endif%d\n", generator.ifLabelCount - 1) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }
  return 0;
}

int generatorIfFalse(){
  char* ptr;
  if (asprintf(&ptr, "LABEL falseif%d\n", generator.ifLabelCount - 1) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}
int generatorIfEnd(){
  char* ptr;

  generator.ifLabelCount--;
  if (asprintf(&ptr, "LABEL endif%d\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  //generator.ifLabelCount++;
  return 0;
}

int generatorWhileStart(){

  if(generator.whileCountMax != 0){
    generator.whileLabelCount = generator.whileCountMax;
  }

  char *ptr;
  if (asprintf(&ptr, "LABEL while%d\n", generator.whileLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}

int generatorWhileBody(){
  char* ptr;
  if (asprintf(&ptr, "JUMPIFEQ whileend%d GF@!bool bool@false\n", generator.whileLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  generator.whileLabelCount++;

  if(generator.whileLabelCount > generator.whileCountMax){
    generator.whileCountMax = generator.whileLabelCount;
  }


  return 0;
}

int generatorWhileEnd(){
  char *ptr;
  generator.whileLabelCount--;

  if (asprintf(&ptr, "JUMP while%d\n", generator.whileLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "LABEL whileend%d\n", generator.whileLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }

  return 0;
}


int generatorFunWrite(){
  char *ptr;

  if (asprintf(&ptr, "CALL $!write%d\n", generator.function_call_cnt) == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    if (realloc_global_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  }

  if (asprintf(&ptr, "LABEL $!write%d\n", generator.function_call_cnt) == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_runtimeCallStr(ptr)) {
   return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_runtimeCallStr(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  for (int i = 0; i < generator.param_count; i++) {
    if (asprintf(&ptr, "DEFVAR TF@param%d\n", i) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_runtimeCallStr(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "POPS TF@param%d\n", i) == -1) {
     return COMP_ERR_INTERNAL;
    }
    if (realloc_runtimeCallStr(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "WRITE TF@param%d\n", i) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_runtimeCallStr(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  }
  if (asprintf(&ptr, "RETURN\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_runtimeCallStr(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  generator.function_call_cnt++;
  return 0;
}

//other builting functions


int generatorFunReads(){
  char *ptr;
  if (asprintf(&ptr, "LABEL reads\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@!ret string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
  return 0;
}

int generatorFunReadi(){
  char *ptr;
  if (asprintf(&ptr, "LABEL readi\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@!ret int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorFunReadf(){
  char *ptr;

  if (asprintf(&ptr, "LABEL readf\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@!ret float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}


int generatorFunStrLen(){

  char* ptr;

  if (asprintf(&ptr, "LABEL strlen\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRLEN GF@!ret TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorFunOrd(){
  //todo add soon

  char* ptr;

  if (asprintf(&ptr, "LABEL ord\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRLEN GF@!bool TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $empty GF@!bool int@0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRI2INT GF@!ret TF@param int@0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMP $notEmpty\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $empty\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "MOVE GF@!ret int@0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $notEmpty\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}


//todo solve problem with negative input

int generatorFunChr(){
  char* ptr;

  if (asprintf(&ptr, "LABEL chr\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "INT2CHAR GF@!ret TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}




int generatorFunSubstring(){

  char* ptr;

  if (asprintf(&ptr, "LABEL substring\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "DEFVAR TF@tmp\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param1\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param2\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param3\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "POPS TF@param1\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "POPS TF@param2\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "POPS TF@param3\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  //if i < 0
  if (asprintf(&ptr, "LT GF@!bool TF@param2 int@0\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $substringNull GF@!bool bool@true\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LT GF@!bool TF@param3 int@0\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "JUMPIFEQ $substringNull GF@!bool bool@true\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "GT GF@!bool TF@param2 TF@param3\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

//last 2

  if (asprintf(&ptr, "JUMPIFEQ $substringNull GF@!bool bool@true\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "STRLEN TF@tmp TF@param1\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LT GF@!bool TF@param2 TF@tmp\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "JUMPIFEQ $substringNull GF@!bool bool@false\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "GT GF@!bool TF@param3 TF@tmp\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "JUMPIFEQ $substringNull GF@!bool bool@true\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;
// here we can copy


  if (asprintf(&ptr, "MOVE GF@!ret string@\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $substringEnd TF@param2 TF@param3\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $substringLoop\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "GETCHAR TF@tmp TF@param1 TF@param2\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "CONCAT GF@!ret GF@!ret TF@tmp\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "ADD TF@param2 TF@param2 int@1\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "JUMPIFNEQ $substringLoop TF@param2 TF@param3\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "JUMP $substringEnd\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $substringNull\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "MOVE GF@!ret nil@nil \n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "LABEL $substringEnd\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_runtimeCallStr(ptr)) return COMP_ERR_INTERNAL;

  return 0;

}

int
generator_finish()
{
  char *ptr;

  if (asprintf(&ptr, "JUMP $!end\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_global_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  /* merge write labels */
  if (realloc_function_def_str(generator.runtimeCallStr)) {
    return COMP_ERR_INTERNAL;
  }

  free(ptr);
  if (asprintf(&ptr, "LABEL $!exit4\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }
  free(ptr);
  if (asprintf(&ptr, "EXIT int@4\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  free(ptr);
  if (asprintf(&ptr, "LABEL $!exit6\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }
  free(ptr);
  if (asprintf(&ptr, "EXIT int@6\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  free(ptr);
  if (asprintf(&ptr, "LABEL $!exit7\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }
  free(ptr);
  if (asprintf(&ptr, "EXIT int@7\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "LABEL $!end") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  return COMP_OK;
}

void
generator_reset()
{
  generator.param_count = 0;
}

char *
get_str_from_type(type type)
{
  /* todo: null */
  char *ret = NULL;

  switch (type) {
  case INT:
    ret = strdup("int");
    break;
  case STRING:
    ret = strdup("string");
    break;
  case FLOAT:
    ret = strdup("float");
    break;
  case QSTRING:
    ret = strdup("qstring");
    break;
  case QINT:
    ret = strdup("qint");
    break;
  case QFLOAT:
    ret = strdup("qfloat");
    break;
  default:
    break;
  }

  return ret;
}

int
generate_param_typecheck(char *p_type, int param_id)
{
  char *ptr;

  if (!strcmp(p_type, "qstring")) {
    /* can be null or a string */
    if (asprintf(&ptr, "JUMPIFNEQ $!%s_str_null%d string@string GF@!type1\n", generator.function_name, param_id) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMP $!%s_param_%d\n", generator.function_name, param_id + 1) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!%s_str_null%d\n", generator.function_name, param_id) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "qint")) {
    /* can be null or a int */
    if (asprintf(&ptr, "JUMPIFNEQ $!%s_int_null%d string@int GF@!type1\n", generator.function_name, param_id) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMP $!%s_param_%d\n", generator.function_name, param_id + 1) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!%s_int_null%d\n", generator.function_name, param_id) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "qfloat")) {
    /* can be null or a float */
    if (asprintf(&ptr, "JUMPIFNEQ $!%s_float_null%d string@float GF@!type1\n", generator.function_name, param_id) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMP $!%s_param_%d\n", generator.function_name, param_id + 1) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!%s_float_null%d\n", generator.function_name, param_id) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "string")) {
    /* can be string */
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@string GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "int")) {
    /* can be int */
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@int GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "float")) {
    /* can be float */
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@float GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  }

  return COMP_OK;
}

int
generate_function_def()
{
  int i;
  char *ptr;
  char *p_type;

  if (asprintf(&ptr, "LABEL %s\n", generator.function_name) == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "PUSHFRAME\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  for (i = 0; i < generator.param_count; i++) {
    /* define the var */
    if (asprintf(&ptr, "DEFVAR LF@%s\n", generator.param_names[i]) == -1) {
    return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    /* load the var from stack */
    if (asprintf(&ptr, "POPS LF@%s\n", generator.param_names[i]) == -1) {
    return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    /* check the type */
    if (asprintf(&ptr, "TYPE GF@!type1 LF@%s\n", generator.param_names[i]) == -1) {
    return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    p_type = get_str_from_type(generator.param_types[i]);
    if (!p_type) {
      /* todo */
      fprintf(stderr, "type err\n");
    }

    if (generate_param_typecheck(p_type, i)) {
      return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "LABEL $!%s_param_%d\n", generator.function_name, i+1) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  }

  if (asprintf(&ptr, "LABEL $!%s_code\n", generator.function_name) == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_local_str_var(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  return COMP_OK;
}

int
generator_function_def_end()
{
  if (generator.local_str_var) {
    if (realloc_function_def_str(generator.local_str_var)) {
      return COMP_ERR_INTERNAL;
    }
  }

  if (generator.local_str) {
    if (realloc_function_def_str(generator.local_str)) {
      return COMP_ERR_INTERNAL;
    }
  }

  free(generator.local_str);
  generator.local_str = NULL;
  free(generator.local_str_var);
  generator.local_str_var = NULL;

  return COMP_OK;
}

int
generate_function_return()
{
  char *ptr;

  if (asprintf(&ptr, "POPFRAME\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "RETURN\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  return COMP_OK;
}

int
return_type_check(type return_type)
{
  char *ptr;
  static int ret_cnt = 0;

  if (asprintf(&ptr, "TYPE GF@!type1 GF@!ret\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_local_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  switch (return_type) {
  case INT:
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@int GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    break;
    case FLOAT:
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@float GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    break;
    case STRING:
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@string GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    break;
    case QSTRING:
    if (asprintf(&ptr, "JUMPIFNEQ $!str_null_%d string@string GF@!type1\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMP $!ret_end_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!str_null_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!ret_end_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    ret_cnt++;
    break;
    case QINT:
    if (asprintf(&ptr, "JUMPIFNEQ $!int_null_%d string@int GF@!type1\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMP $!ret_end_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!int_null_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!ret_end_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    ret_cnt++;
    break;
    case QFLOAT:
    if (asprintf(&ptr, "JUMPIFNEQ $!flt_null_%d string@float GF@!type1\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMP $!ret_end_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!flt_null_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@!type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    if (asprintf(&ptr, "LABEL $!ret_end_%d\n", ret_cnt) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    ret_cnt++;
    break;
  default:
    break;
  }

  return COMP_OK;
}

int
defvar_local(const char *var_name)
{
  char *ptr;

  if (asprintf(&ptr, "DEFVAR LF@%s\n", var_name) == -1) {
    return COMP_ERR_INTERNAL;
  }

  if (!generator.local_str_var || !strstr(generator.local_str_var, ptr) || !strstr(generator.local_str_var, ptr)) {
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  }

  return COMP_OK;
}

