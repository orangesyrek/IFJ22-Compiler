#include "generator.h"

struct generator generator = {0};

unsigned long id = 0;

void generatorInit(){
  printf(".IFJcode22\n");
  printf("CREATEFRAME\n");
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
void generatorPushParamString(char *str){
  printf("PUSHS string@%s\n", str);
}

void generatorExecute(char *fun){ // need to know function name
  printf("PUSHFRAME\n");
  for (int i = 0; i < generator.param_count; i++) {
    if (generator.params[i].type == INT) {
      printf("PUSHS int@%d\n", generator.params[i].value.int_val);
    } else if (generator.params[i].type == FLOAT) {
      printf("PUSHS float@%a\n", generator.params[i].value.flt_val);
    } else if (generator.params[i].type == STRING) {
      printf("PUSHS string@%s\n", generator.params[i].value.str_val);
    }
  }
  printf("CALL %s%d\n", fun, generator.function_call_cnt);
  generatorFunWrite(); // to test
  printf("POPFRAME\n");// maybe delete depends
}

void generatorBuiltinFunctions(){
  generatorFunWrite();
  generatorFunReadf();
  generatorFunReadi();
  generatorFunReads();
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


void generatorFunReads(){
  printf("JUMP $readsend\n");
  printf("LABEL reads\n");
  printf("CREATEFRAME\n"); // maybe not needed too
  printf("READ LF@ret string\n");
  printf("RETURN\n");
  printf("LABEL $readsend\n");
}

void generatorFunReadi(){
  printf("JUMP $readiend\n");
  printf("LABEL readi\n");
  printf("CREATEFRAME\n"); // maybe not needed too
  printf("READ LF@ret int\n");
  printf("RETURN\n");
  printf("LABEL $readiend\n");
}

void generatorFunReadf(){
  printf("JUMP $readfend\n");
  printf("LABEL readf\n");
  printf("CREATEFRAME\n"); // maybe not needed too
  printf("READ LF@ret float\n");
  printf("RETURN\n");
  printf("LABEL $readfend\n");
}


void generatorFunStrLen(){
  printf("JUMP $strlenend\n");
  printf("LABEL strlen\n");
  printf("CREATEFRAME\n");
  printf("DEFVAR TF@input\n");
  printf("POPS TF@input\n");
  printf("STRLEN LF@ret TF@input\n");
  printf("RETURN\n");
  printf("LABEL $strlenend\n");
}


/*todo substring | ord | chr*/


