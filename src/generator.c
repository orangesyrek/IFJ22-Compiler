#include "generator.h"

unsigned long id = 0;

void generatorInit(){
  printf(".IFJcode22\n");
  printf("CREATEFRAME\n");
  //printf("DEFVAR GF@bool");
}

void generatorPrepare(struct bs_data *functionData){
  printf("PUSHFRAME");
  printf("CREATEFRAME\n");

  if(functionData->data.fdata.return_type != VOID){
    printf("DEFVAR TF@ret");
  }
  if(functionData->data.fdata.param_count!= 0){
    printf("DEFVAR TF@pcount");
    printf("MOVE TF@pcount int@%d\n", functionData->data.fdata.param_count);
  }
}
//not added

char* generatorCalculateId(){
char idstr[1024]; // need some dynamic string or strdub
sprintf(idstr, "%lu", id);
char *name = strcat("p", idstr);
return name;
}

//now only for strings, then need to rebuilt to take float, int, bool null
//only for non-built in functions
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



void generatorFunWrite(){
  printf("LABEL write\n");
  printf("CREATEFRAME\n");
  //printf("DEFVAR TF@cnt_of_parameter\n");
  printf("DEFVAR TF@to_print\n");
  printf("DEFVAR TF@cnt\n");
  //printf("POPS TF@cnt_of_parameter\n");
  printf("MOVE TF@cnt int@0\n");
  printf("LABEL _print_while_start\n");
  //printf("LT GF@bool TF@cnt LF@pcount\n");
  printf("JUMPIFEQ _print_while_end TF@cnt LF@pcount\n");
  printf("POPS TF@to_print\n");
  //idk about this part *** maybe for for not to empty stack if bad count of params come
  printf("JUMPIFNEQ exprint TF@to_print nil@nil \n");
  printf("PUSHS string@nil\n");
  printf("POPS TF@to_print\n");
  printf("LABEL exprint\n");
  //                    ***
  printf("WRITE TF@to_print\n");
  printf("ADD TF@cnt TF@cnt int@1\n");
  printf("JUMP _print_while_start\n");
  printf("LABEL _print_while_end\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
}

void generatorExecute(char *fun){ // need to know function name
  printf("PUSHFRAME");
  printf("CALL %s\n", fun);
  generatorFunWrite();
  printf("POPFRAME");
}














