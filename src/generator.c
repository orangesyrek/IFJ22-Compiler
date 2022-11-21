#include "generator.h"


char* convertString(char* nonConvertedstr){

  char* converted =(char*) malloc(512);
  if(nonConvertedstr != NULL){
    int index = 0;
    char i = nonConvertedstr[index];
		int j = 0;
    while(i != '\0'){
      if( (i <= 32) || (i == 35) || (i == 92) ){
				converted[j] = 92;
				converted[j+1] = 48;
				converted[j+2] = (i / 10) + 48;
				converted[j+3] = (i % 10) + 48;
				j += 4;

      }else{
				converted[j] = i;
				j++;
	    }
      i = nonConvertedstr[++index];
    }

  return converted;

  }else{
    return NULL;
  }
}

unsigned long id = 0;

void generatorInit(){
  printf(".IFJcode22\n");
  printf("CREATEFRAME\n");
  printf("GF@ret\n"); // need some more global variables
  //printf("DEFVAR GF@bool");
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
  printf("CALL %s\n", fun);
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
  printf("JUMP $writeend\n");
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
      // printf("JUMPIFNEQ exprint TF@to_print nil@nil \n");
      // printf("PUSHS string@nil\n");
      // printf("POPS TF@to_print\n");
      // printf("LABEL exprint\n");
  //                    ***
  printf("WRITE TF@to_print\n");
  printf("ADD TF@cnt TF@cnt int@1\n");
  printf("JUMP _print_while_start\n");
  printf("LABEL _print_while_end\n");
  //printf("POPFRAME\n");
  printf("RETURN\n");
  printf("LABEL $writeend\n");
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


