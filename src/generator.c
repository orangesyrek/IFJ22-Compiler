#define _GNU_SOURCE /* strdup */
#include "generator.h"
#include "compiler.h"
#define _GNU_SOURCE

struct generator generator = {0};

/*
Todo

typecasting other builtin functions
if else blocks
expressions
think about pushing params inside function

*/

/*
someinfo
used local_str as tmp str value need to change that to something else
*/
unsigned long id = 0;

void generatorInit(){
  char* ptr;
  if (realloc_local_str("")) exit(COMP_ERR_INTERNAL);
  if (realloc_local_str_var("")) exit(COMP_ERR_INTERNAL);
  if (realloc_function_def_str("")) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str("")) exit(COMP_ERR_INTERNAL);
  if (realloc_write_str("")) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, ".IFJcode22\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);


  if (asprintf(&ptr, "CREATEFRAME\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);


  if (asprintf(&ptr, "DEFVAR GF@bool\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@tmp1\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@tmp2\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@type1\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@type2\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@ret\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (generatorFunReads()) {
    exit(COMP_ERR_INTERNAL);
  }
  if (generatorFunReadi()) {
    exit(COMP_ERR_INTERNAL);
  }
  if (generatorFunReadf()) {
    exit(COMP_ERR_INTERNAL);
  }
  if (generatorFunOrd()) {
    exit(COMP_ERR_INTERNAL);
  }
  if (generatorFunChr()) {
    exit(COMP_ERR_INTERNAL);
  }
  if (generatorFunStrLen()) {
    exit(COMP_ERR_INTERNAL);
  }
  if (generatorFunSubstring()) {
    exit(COMP_ERR_INTERNAL);
  }
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
    default:
      converted[*position] = character;
      *position+=1;
      break;
  }
}

char* convertString(char* nonConvertedstr){
  char* converted =(char*) calloc(1, 512);
  if(nonConvertedstr != NULL){
    int index = 0;
    char character = nonConvertedstr[index];
    int position = 0;
    while(character != '\0'){
      if( (character <= 32) || (character == 35) ){
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

  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

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

int realloc_write_str(const char *str) {

  if (!generator.write_str) {
    generator.write_str = strdup(str);
    if (!generator.write_str) {
      return COMP_ERR_INTERNAL;
    }
  } else {
    generator.write_str = realloc(generator.write_str, strlen(generator.write_str) + strlen(str) + 1);
    if (!generator.write_str) {
      return COMP_ERR_INTERNAL;
    }
    strcat(generator.write_str, str);
  }

  return 0;
}



int generatorAssigment(const char* var_name){
  char* ptr;
  //case globar var | local var
  //then need to solve typecasting
  if(generator.inFuntion){
    //printf("MOVE GF@%s GF@ret\n", var_name);
    if (asprintf(&ptr, "MOVE LF@%s GF@ret\n", var_name) == -1) return COMP_ERR_INTERNAL;
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  }else{
    if (asprintf(&ptr, "MOVE GF@%s GF@ret\n", var_name) == -1) return COMP_ERR_INTERNAL;
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
    //printf("MOVE LF@%s GF@ret\n", var_name);
  }
  return 0;

}

void generatorPrepare(struct bs_data *functionData){ // redo to fdata
  printf("PUSHFRAME");
  printf("CREATEFRAME\n");

  if(functionData->data.fdata.return_type != VOID){
    printf("DEFVAR TF@ret\n");// can leave it here
  }
  if(functionData->data.fdata.param_count!= 0){
    printf("DEFVAR TF@pcount\n");
    printf("MOVE TF@pcount int@%d\n", functionData->data.fdata.param_count);
  }
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
        if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == FLOAT) {
        if (asprintf(&ptr, "PUSHS float@%a\n", generator.params[i].value.flt_val) == -1) return COMP_ERR_INTERNAL;
        if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == STRING) {
        str = convertString(generator.params[i].value.str_val);
        if (!str) {
          return COMP_ERR_INTERNAL;
        }
        if (asprintf(&ptr, "PUSHS string@%s\n", str) == -1) return COMP_ERR_INTERNAL;
        if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == UNKNOWN) {
          if (asprintf(&ptr, "PUSHS GF@%s\n", generator.params[i].value.var_name) == -1) {
            return COMP_ERR_INTERNAL;
          }
          if (realloc_function_def_str(ptr)) {
           return COMP_ERR_INTERNAL;
          }
      } else if (generator.params[i].type == T_NULL) {
        if (asprintf(&ptr, "PUSHS nil@nil\n") == -1) {
            return COMP_ERR_INTERNAL;
        }
        if (realloc_function_def_str(ptr)) {
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

  if (asprintf(&ptr, "PUSHFRAME\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if(generatorPushParam()){
    exit(COMP_ERR_INTERNAL);
  }

  /* only need id for write */
  if (!strcmp("write", generator.function_name)) {
    generatorFunWrite();
  } else {
    if (asprintf(&ptr, "CALL %s\n", generator.function_name) == -1) exit(COMP_ERR_INTERNAL);
    if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);
  }

  if (asprintf(&ptr, "POPFRAME\n") == -1) exit(COMP_ERR_INTERNAL); // maybe delete depends
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);
}

void generatorWriteCode(){
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
      if (asprintf(&ptr, "PUSHS string@%s\n", token.value.str_val) == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == DECIMAL_LIT) {
      if (asprintf(&ptr, "PUSHS float@%a\n", token.value.flt_val) == -1) return COMP_ERR_INTERNAL;
      if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
    }else if (token.type == VAR) {
      // difference betweem global var and local
      if (asprintf(&ptr, "PUSHS GF@%s\n", token.id) == -1) return COMP_ERR_INTERNAL;
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
      if (asprintf(&ptr, "PUSHS string@%s\n", token.value.str_val) == -1) return COMP_ERR_INTERNAL;
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

int generatorExprPlus(){
  char* ptr;
  if (asprintf(&ptr, "ADDS\n") == -1) return COMP_ERR_INTERNAL;

  if (generator.inFuntion) {
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  } else {
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  }
  return 0;
}

int generatorExprMinus(){
  char* ptr;
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

  if (asprintf(&ptr, "DIVS\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  return 0;
}

int generatorExprConcat(){
  char* ptr;


  if (asprintf(&ptr, "POPS GF@tmp2\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
 //todo check if its string
 if (asprintf(&ptr, "POPS GF@tmp1\n") == -1) return COMP_ERR_INTERNAL;
 if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

 if (asprintf(&ptr, "CONCAT GF@ret GF@tmp1 GF@tmp2\n") == -1) return COMP_ERR_INTERNAL;
 if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

 if (asprintf(&ptr, "PUSHS GF@ret\n") == -1) return COMP_ERR_INTERNAL;
 if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}


int generatorExpressionCalculated(){
  char* ptr;
  if(!generator.isIf){
    if (asprintf(&ptr, "POPS GF@ret\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  }else{
    if (asprintf(&ptr, "PUSHS int@0\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "POPS GF@ret\n") == -1) return COMP_ERR_INTERNAL;
    if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  }
  return 0;
}

int generatorIfEquals(){

  char* ptr;
  if (asprintf(&ptr, "EQS\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS GF@bool\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;


  return 0;
}
int generatorIfTrue(){
  if(generator.ifCountMax != 0 && generator.ifLabelCount == 0){
    generator.ifLabelCount = generator.ifCountMax;
  }
  char* ptr;
  if (asprintf(&ptr, "LABEL trueif%d\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ falseif%d GF@bool bool@false\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  generator.ifLabelCount++;
  if(generator.ifLabelCount > generator.ifCountMax){
    generator.ifCountMax = generator.ifLabelCount;
  }

  return 0;
}

int generatorIfTrueEnd(){

  generator.ifLabelCount--;

  char* ptr;
  if (asprintf(&ptr, "JUMP endif%d\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
  return 0;
}

int generatorIfFalse(){
  char* ptr;
  if (asprintf(&ptr, "LABEL falseif%d\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;


  return 0;
}
int generatorIfEnd(){
  char* ptr;
  if (asprintf(&ptr, "LABEL endif%d\n", generator.ifLabelCount) == -1) return COMP_ERR_INTERNAL;
  if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;

  //generator.ifLabelCount++;
  return 0;
}


int generatorFunWrite(){
  char *ptr;

  if (asprintf(&ptr, "CALL $!write%d\n", generator.function_call_cnt) == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (generator.inFuntion) {
    if (realloc_function_def_str(ptr)) {
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
  if (realloc_write_str(ptr)) {
   return COMP_ERR_INTERNAL;
  }

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_write_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  for (int i = 0; i < generator.param_count; i++) {
    if (asprintf(&ptr, "DEFVAR TF@param%d\n", i) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_write_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "POPS TF@param%d\n", i) == -1) {
     return COMP_ERR_INTERNAL;
    }
    if (realloc_write_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }

    if (asprintf(&ptr, "WRITE TF@param%d\n", i) == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_write_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  }
  if (asprintf(&ptr, "RETURN\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_write_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  generator.function_call_cnt++;
  return 0;
}

//other builting functions


int generatorFunReads(){
  char *ptr;
  if (asprintf(&ptr, "LABEL reads\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@ret string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
  return 0;
}

int generatorFunReadi(){
  char *ptr;
  if (asprintf(&ptr, "LABEL readi\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@ret int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorFunReadf(){
  char *ptr;

  if (asprintf(&ptr, "LABEL readf\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@ret float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}


int generatorFunStrLen(){

  char* ptr;

  if (asprintf(&ptr, "LABEL strlen\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRLEN GF@ret TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}

int generatorFunOrd(){
  //todo add soon

  char* ptr;

  if (asprintf(&ptr, "LABEL ord\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRLEN GF@bool TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMPIFEQ $empty GF@bool int@0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRI2INT GF@ret TF@param int@0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "JUMP $notEmpty\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $empty\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "MOVE GF@ret int@0\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $notEmpty\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}


//todo solve problem with negative input

int generatorFunChr(){
  char* ptr;

  if (asprintf(&ptr, "LABEL chr\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "INT2CHAR GF@ret TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  return 0;
}




int generatorFunSubstring(){

  /*todo substring */
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
  if (realloc_function_def_str(generator.write_str)) {
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
    if (asprintf(&ptr, "JUMPIFNEQ $!%s_str_null%d string@string GF@type1\n", generator.function_name, param_id) == -1) {
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
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "qint")) {
    /* can be null or a int */
    if (asprintf(&ptr, "JUMPIFNEQ $!%s_int_null%d string@int GF@type1\n", generator.function_name, param_id) == -1) {
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
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "qfloat")) {
    /* can be null or a float */
    if (asprintf(&ptr, "JUMPIFNEQ $!%s_float_null%d string@float GF@type1\n", generator.function_name, param_id) == -1) {
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
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@nil GF@type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "string")) {
    /* can be string */
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@string GF@type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "int")) {
    /* can be int */
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@int GF@type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_local_str_var(ptr)) {
      return COMP_ERR_INTERNAL;
    }
  } else if (!strcmp(p_type, "float")) {
    /* can be float */
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@float GF@type1\n") == -1) {
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
    if (asprintf(&ptr, "TYPE GF@type1 LF@%s\n", generator.param_names[i]) == -1) {
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

  if (realloc_function_def_str(generator.local_str_var)) {
    return COMP_ERR_INTERNAL;
  }

  if (realloc_function_def_str(generator.local_str)) {
    return COMP_ERR_INTERNAL;
  }

  free(generator.local_str_var);
  generator.local_str_var = NULL;
  free(generator.local_str);
  generator.local_str = NULL;

  return COMP_OK;
}

int
generate_function_return()
{
  char *ptr;

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

  if (asprintf(&ptr, "TYPE GF@type1 GF@ret\n") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_function_def_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  switch (return_type) {
  case INT:
    if (asprintf(&ptr, "JUMPIFNEQ $!exit4 string@int GF@type1\n") == -1) {
      return COMP_ERR_INTERNAL;
    }
    if (realloc_function_def_str(ptr)) {
      return COMP_ERR_INTERNAL;
    }
    break;
  default:
    break;
  }

  return COMP_OK;
}
