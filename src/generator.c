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

  if (asprintf(&ptr, ".IFJcode22\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);


  if (asprintf(&ptr, "CREATEFRAME\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (asprintf(&ptr, "DEFVAR GF@bool\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);


  if (asprintf(&ptr, "DEFVAR GF@ret\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

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

  if(generatorAssigment(var_name, 1)) return COMP_ERR_INTERNAL;
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


int generatorAssigment(const char* var_name, int isGlobal){
  char* ptr;
  //case globar var | local var
  //then need to solve typecasting
  if(isGlobal){
    //printf("MOVE GF@%s GF@ret\n", var_name);
    if (asprintf(&ptr, "MOVE GF@%s GF@ret\n", var_name) == -1) return COMP_ERR_INTERNAL;
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
  }else{
    if (asprintf(&ptr, "MOVE LF@%s GF@ret\n", var_name) == -1) return COMP_ERR_INTERNAL;
    if (realloc_local_str(ptr)) return COMP_ERR_INTERNAL;
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
  if(generator.inFuntion){
    // todo
    return 0;
  }else{
//    printf("%s%d\n", "problem", generator.param_count);
    char* ptr;
    for (int i = 0; i < generator.param_count; i++) {
      if (generator.params[i].type == INT) {
        //printf("PUSHS int@%d\n", generator.params[i].value.int_val);
        if (asprintf(&ptr, "PUSHS int@%d\n", generator.params[i].value.int_val) == -1) return COMP_ERR_INTERNAL;
        if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == FLOAT) {
        //printf("PUSHS float@%a\n", generator.params[i].value.flt_val);
        if (asprintf(&ptr, "PUSHS float@%a\n", generator.params[i].value.flt_val) == -1) return COMP_ERR_INTERNAL;
        if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
      } else if (generator.params[i].type == STRING) {
        char* str;
        str = convertString(generator.params[i].value.str_val);
        if (!str) {
          return COMP_ERR_INTERNAL;
        }
        if (asprintf(&ptr, "PUSHS string@%s\n", str) == -1) return COMP_ERR_INTERNAL;
        if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
        //printf("PUSHS string@%s\n", str);
      } else if (generator.params[i].type == UNKNOWN) {
          //need to solve convertions types
            if (asprintf(&ptr, "PUSHS GF@%s\n", generator.params[i].value.var_name) == -1) return COMP_ERR_INTERNAL;
            if (realloc_global_str(ptr)) return COMP_ERR_INTERNAL;
      }else{
      }
    }
    return 0;
  }
  //printf("PUSHS string@%s\n", str);
}

void generatorExecute(){ // need to know function name
  //generator.inFuntion = 0; // SA should tell
  char* ptr;
  //printf("PUSHFRAME\n");
  if (asprintf(&ptr, "PUSHFRAME\n") == -1) exit(COMP_ERR_INTERNAL);
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);


  if(generatorPushParam()){
    exit(COMP_ERR_INTERNAL);
  }

  if (asprintf(&ptr, "CALL %s%d\n", generator.function_name, generator.function_call_cnt) == -1) exit(COMP_ERR_INTERNAL);;
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);;

  if (asprintf(&ptr, "POPFRAME\n") == -1) exit(COMP_ERR_INTERNAL); // maybe delete depends
  if (realloc_global_str(ptr)) exit(COMP_ERR_INTERNAL);

  if (generateBuiltInFunc(generator.function_name)) exit(COMP_ERR_INTERNAL);
  //generatorFunWriteR(); // to test

}

int generateBuiltInFunc(char* funName){
  if(strcmp(funName, "write") == 0){
    if(generatorFunWriteR()) return COMP_ERR_INTERNAL;
  }else if (strcmp(funName, "reads") == 0){
    if (generatorFunReads()) return COMP_ERR_INTERNAL;
  }
  else if (strcmp(funName, "readi") == 0){
    if (generatorFunReadi()) return COMP_ERR_INTERNAL;
  }
  else if (strcmp(funName, "readf") == 0){
    if (generatorFunReadf()) return COMP_ERR_INTERNAL;
  }
  else if (strcmp(funName, "strlen") == 0){
    if (generatorFunStrLen()) return COMP_ERR_INTERNAL;
    return 0;
  }
  else if (strcmp(funName, "substring") == 0){
    //todo
    return 0;
  }
  else if (strcmp(funName, "ord") == 0){
    if (generatorFunOrd()) return COMP_ERR_INTERNAL;
    return 0;
  }
  else if (strcmp(funName, "chr") == 0){
    //todo
    return 0;
  }
  return 0;
}

void generatorWriteCode(){
  printf("%s\n", generator.global_str);
  printf("%s\n", generator.local_str_var);
  printf("%s\n", generator.local_str);
  printf("%s\n", generator.function_def_str);
}

int generatorFunWriteR(){
  char *ptr;
  if (asprintf(&ptr, "JUMP $writeend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
  if (asprintf(&ptr, "LABEL write%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
  for (int i = 0; i < generator.param_count; i++) {
    if (asprintf(&ptr, "DEFVAR TF@param%d\n", i) == -1) return COMP_ERR_INTERNAL;
    if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "POPS TF@param%d\n", i) == -1) return COMP_ERR_INTERNAL;
    if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

    if (asprintf(&ptr, "WRITE TF@param%d\n", i) == -1) return COMP_ERR_INTERNAL;
    if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;


  }
  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;
  if (asprintf(&ptr, "LABEL $writeend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  generator.function_call_cnt++;
  free(ptr);
  return 0;
}

void generatorFunWrite(){
  printf("JUMP $writeend%d\n", generator.function_call_cnt);
  printf("LABEL write%d\n", generator.function_call_cnt);
  printf("CREATEFRAME\n");
  for (int i = 0; i < generator.param_count; i++) {
    printf("DEFVAR TF@param%d\n", i);
    printf("POPS TF@param%d\n", i);
    printf("WRITE TF@param%d\n", i);
  }
  printf("RETURN\n");
  printf("LABEL $writeend%d\n", generator.function_call_cnt);
  generator.function_call_cnt++;
}

//other builting functions


int generatorFunReads(){
  char *ptr;
  if (asprintf(&ptr, "JUMP $readsend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL reads%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@ret string\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $readsend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  //interesting way
  if (realloc_global_str(generator.local_str)) return COMP_ERR_INTERNAL;
  free(generator.local_str);
  generator.local_str = NULL;
  if (realloc_local_str("")) return COMP_ERR_INTERNAL;

  generator.function_call_cnt++;
  return 0;
}

int generatorFunReadi(){
  char *ptr;
  if (asprintf(&ptr, "JUMP $readseni%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL readi%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@ret int\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $readseni%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  //interesting way
  if (realloc_global_str(generator.local_str)) return COMP_ERR_INTERNAL;
  free(generator.local_str);
  generator.local_str = NULL;
  if (realloc_local_str("")) return COMP_ERR_INTERNAL;

  generator.function_call_cnt++;
  return 0;
}

int generatorFunReadf(){
  char *ptr;
  if (asprintf(&ptr, "JUMP $readsenf%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL readf%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "READ GF@ret float\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;


  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL $readsenf%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  //interesting way
  if (realloc_global_str(generator.local_str)) return COMP_ERR_INTERNAL;
  free(generator.local_str);
  generator.local_str = NULL;
  if (realloc_local_str("")) return COMP_ERR_INTERNAL;

  generator.function_call_cnt++;
  return 0;
}


int generatorFunStrLen(){

  char* ptr;
  if (asprintf(&ptr, "JUMP $strlenend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL strlen%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

//  printf("JUMP $strlenend\n");
//  printf("LABEL strlen\n");
//  printf("CREATEFRAME\n");
  if (asprintf(&ptr, "CREATEFRAME\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "DEFVAR TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "POPS TF@param\n" ) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "STRLEN GF@ret TF@param\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  //printf("STRLEN LF@ret TF@input\n");

  if (asprintf(&ptr, "RETURN\n") == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;


//  printf("RETURN\n");
  if (asprintf(&ptr, "LABEL $strlenend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (realloc_global_str(generator.local_str)) return COMP_ERR_INTERNAL;
  free(generator.local_str);
  generator.local_str = NULL;
  if (realloc_local_str("")) return COMP_ERR_INTERNAL;


  generator.function_call_cnt++;
  return 0;
//  printf("LABEL $strlenend\n");
}

int generatorFunOrd(){
  //todo add soon

  char* ptr;
  if (asprintf(&ptr, "JUMP $ordend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (asprintf(&ptr, "LABEL ord%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  //  printf("JUMP $strlenend\n");
  //  printf("LABEL strlen\n");
  //  printf("CREATEFRAME\n");
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

  //  printf("RETURN\n");
  if (asprintf(&ptr, "LABEL $ordlenend%d\n", generator.function_call_cnt) == -1) return COMP_ERR_INTERNAL;
  if (realloc_function_def_str(ptr)) return COMP_ERR_INTERNAL;

  if (realloc_global_str(generator.local_str)) return COMP_ERR_INTERNAL;
  free(generator.local_str);
  generator.local_str = NULL;
  if (realloc_local_str("")) return COMP_ERR_INTERNAL;


  generator.function_call_cnt++;
  return 0;


  //return 0;
}


/*todo substring | ord | chr*/

int
generator_finish()
{
  char *ptr;

  if (asprintf(&ptr, "JUMP $#_end") == -1) {
    return COMP_ERR_INTERNAL;
  }
  if (realloc_global_str(ptr)) {
    return COMP_ERR_INTERNAL;
  }

  free(ptr);
  if (asprintf(&ptr, "LABEL $#_end") == -1) {
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
