#define _GNU_SOURCE /* strdup */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "compiler.h"
#include "lexical_analysis.h"

extern struct compiler_ctx *ctx;

//just testing start ************************

char* dynamicString = NULL;
int dynamicStringOffset = 0;


/*
initializes dynamic string
return 0 if everythink is OK else 1
*/
int initDynString(){
  dynamicString = (char*) malloc(sizeof(char));
  if(dynamicString == NULL)
  {
    return 1;
  }else
  {
    return 0;
  }
}
/*
Destroyes dynamic string
*/
void destroyDynString(){
  if(dynamicString != NULL){
    free(dynamicString);
  }
}

/*
Realocates dynamic string by 1
return 0 if everythink is OK else 1
*/
int realocateDynString(){
  dynamicString = (char*) realloc(dynamicString, 1);
  if(dynamicString == NULL)
  {
    return 1;
  }else
  {
    return 0;
  }

}

//just testing end ************************

/*
Debbuging function to see what token is made
*/

void printToken(struct lexeme lex){
  switch(lex.type){
    case FUN_ID:
        printf("%s%s\n", "FUN_ID: ", lex.id);
        break;
    case STR_LIT:
        printf("%s\n", "STR_LIT");
        break;
    case ASSIGNMENT:
        printf("%s\n", "ASSIGNMENT");
        break;
    case INT_LIT:
        printf("%s\n", "INT_LIT");
        break;
    case DECIMAL_LIT:
        printf("%s\n", "DECIMAL_LIT");
        break;
    case VAR:
        printf("%s%s\n", "VAR: ",lex.id);
        break;
    case L_PAR:
        printf("%s\n", "L_PAR");
        break;
    case R_PAR:
        printf("%s\n", "R_PAR");
        break;
    case SEMICOLON:
        printf("%s\n", "SEMICOLON");
        break;
    case L_CURLY:
        printf("%s\n", "L_CURLY");
        break;
    case R_CURLY:
        printf("%s\n", "R_CURLY");
        break;
    case REL_IDENTICAL:
      printf("%s\n", "REL_IDENTICAL");
      break;
    case REL_NEQ:
      printf("%s\n", "REL_NEQ");
      break;
    case REL_LESS:
        printf("%s\n", "REL_LESS");
        break;
    case REL_LESS_EQ:
        printf("%s\n", "REL_LESS_EQ");
        break;
    case REL_GREATER:
        printf("%s\n", "REL_GREATER");
        break;
    case REL_GREATER_EQ:
        printf("%s\n", "REL_GREATER_EQ");
        break;
    case PLUS:
        printf("%s\n", "PLUS");
        break;
    case MINUS:
        printf("%s\n", "MINUS");
        break;
    case ASTERISK:
        printf("%s\n", "ASTERISK");
        break;
    case SLASH:
        printf("%s\n", "SLASH");
        break;
    case DOT:
        printf("%s\n", "DOT");
        break;
    case COMMA:
        printf("%s\n", "COMMA");
        break;
    case COLON:
        printf("%s\n", "COLON");
        break;
    case PARAM_TYPE:
        printf("%s\n", "PARAM_TYPE");
        break;
    case COMMENT:
        printf("%s\n", "COMMENT");
        break;
    case PROLOG_START:
        printf("%s\n", "PROLOG_START");
        break;
    case PROLOG_END:
        printf("%s\n", "PROLOG_END");
        break;
    case LEX_EOF:
        printf("%s\n", "LEX_EOF");
        break;
    case KEYWORD_IF:
        printf("%s\n", "KEYWORD_IF");
        break;
    case KEYWORD_ELSE:
        printf("%s\n", "KEYWORD_ELSE");
        break;
    case KEYWORD_FUNCTION:
        printf("%s\n", "KEYWORD_FUNCTION");
        break;
    case KEYWORD_NULL:
        printf("%s\n", "KEYWORD_NULL");
        break;
    case KEYWORD_RETURN:
        printf("%s\n", "KEYWORD_RETURN");
        break;
    case KEYWORD_VOID:
        printf("%s\n", "KEYWORD_VOID");
        break;
    case KEYWORD_WHILE:
        printf("%s\n", "KEYWORD_WHILE");
        break;
    case KEYWORD_DECLARE:
        printf("%s\n", "KEYWORD_DECLARE");
        break;
    case KEYWORD_STRICT_TYPES:
        printf("%s\n", "KEYWORD_STRICT_TYPES");
        break;
    case ERROR_LEX:
        printf("%s\n", "ERROR_LEX");
        break;

  }
}

/*
Function that converts function ID to keyword if it is keyword
return keyword or function id
*/
lex_types funIdToKeyword(char* id)
{
    if(strcmp(id, "if") == 0){
      return KEYWORD_IF;
    }else if(strcmp(id, "else") == 0){
      return KEYWORD_ELSE;
    }else if(strcmp(id, "null") == 0){
      return KEYWORD_NULL;
    }else if(strcmp(id, "return") == 0){
      return KEYWORD_RETURN;
    }else if(strcmp(id, "void") == 0){
      return KEYWORD_VOID;
    }else if(strcmp(id, "while") == 0){
      return KEYWORD_WHILE;
    }else if(strcmp(id, "declare") == 0){
      return KEYWORD_DECLARE;
    }else if(strcmp(id, "strict_types") == 0){
      return KEYWORD_STRICT_TYPES;
    } else if (!strcmp(id, "function")) {
        return KEYWORD_FUNCTION;
    } else if ((!strcmp(id, "int")) || (!strcmp(id, "float")) || (!strcmp(id, "string"))) {
        return PARAM_TYPE;
    }
    else{
      return FUN_ID;
    }
}


void print_ctx()
{

    printf("%d %d\n", ctx->last_read, ctx->current_row);
    ctx->current_row = 1;
}

// void *
// la_realloc(void *ptr, size_t size)
// {
//     void *ret;
//
//     ret = realloc(ptr, size);
//     if (!ret) {
//         free(ptr);
//     }
//
//     return ret;
// }


static
lex_types makeLexeme(state final) /* where lexemes are generated, can generate only if you are in state where you can generate something */
{
    switch(final){
        case SLASH_STATE:
            return SLASH;
        case COMMENT_END:
            return COMMENT;
        case VAR_STATE:
            return VAR;
        case SEMICOLON_STATE:
            return SEMICOLON;
        case R_CURLY_STATE:
            return R_CURLY;
        case L_CURLY_STATE:
            return L_CURLY;
        case REL_LESS_STATE:
            return REL_LESS;
        case REL_LESS_EQ_STATE:
            return REL_LESS_EQ;
        case PROLOG_STATE:
            return PROLOG_START;
        case REL_GREATER_STATE:
            return REL_GREATER;
        case REL_GREATER_EQ_STATE:
            return REL_GREATER_EQ;
        case PLUS_STATE:
            return PLUS;
        case MINUS_STATE:
            return MINUS;
        case ASTERISK_STATE:
            return ASTERISK;
        case DOT_STATE:
            return DOT;
        case COMMA_STATE:
            return COMMA;
        case COLON_STATE:
            return COLON;
        case L_PAR_STATE:
            return L_PAR;
        case R_PAR_STATE:
            return R_PAR;
        case PROLOG_END_STATE:
            return PROLOG_END;
        case FUN_ID_STATE:
            return FUN_ID;
        case INT_LIT_STATE:
            return INT_LIT;
        case DECIMAL_LIT_STATE:
            return DECIMAL_LIT;
        case DEC_LIT_E_TMP:
            return DECIMAL_LIT;
        case STR_LIT_STATE:
            return STR_LIT;
        case ASSIGNMENT_STATE:
            return ASSIGNMENT;
        case REL_IDENTICAL_STATE:
            return REL_IDENTICAL;
        case REL_NEQ_STATE:
            return REL_NEQ;
        case MULTI_LINE_COMMENT:
            return COMMENT;
        case LEX_EOF_STATE:
            return LEX_EOF;
        case ERROR_STATE:
            return ERROR_LEX;
        default:
            return 1;

    };
}
static
state getNextState(state currentState, int input) {  /* decide what is next state based on input and current state */
    switch(currentState) {
        case ERROR_STATE:
            fprintf(stderr, "Should have generated token by now\n");
            return ERROR_STATE;
        case Start:
            if (isspace(input)){
                return Start;
            }
            else if (input == '/'){
                return SLASH_STATE;
            }
            else if (input == '$'){
                return VAR_PREFIX;
            }
            else if (input == ';'){
                return SEMICOLON_STATE;
            }
            else if (input == '}'){
                return R_CURLY_STATE;
            }
            else if (input == '{'){
                return L_CURLY_STATE;
            }
            else if (input == '<'){
                 return REL_LESS_STATE;
            }
            else if (input == '>'){
                return REL_GREATER_STATE;
            }
            else if (input == '+'){
                return PLUS_STATE;
            }
            else if (input == '-'){
                return MINUS_STATE;
            }
            else if (input == '*'){
                return ASTERISK_STATE;
            }
            else if (input == '.'){
                return DOT_STATE;
            }
            else if (input == ','){
                return COMMA_STATE;
            }
            else if (input == ':'){
                return COLON_STATE;
            }
            else if (input == '('){
                return L_PAR_STATE;
            }
            else if (input == ')'){
                return R_PAR_STATE;
            }
            else if (input == '?'){
                return QUESTION_MARK;
            }
            else if (isalpha(input) || input=='_'){
                 return FUN_ID_STATE;
            }
            else if (isdigit(input)){
                return INT_LIT_STATE;
            }
            else if (input == '"'){
                return STR_LIT_STATE;
            }
            else if (input == '='){
                return ASSIGNMENT_STATE;
            }
            else if (input == '!'){
                return EXCLAMATION_MARK;
            } else if(input == EOF){
                return LEX_EOF_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case SLASH_STATE:
            if (input == '*'){
                return MULTI_LINE_COMMENT;
            }
            else if (input == '/'){
                return ONE_LINE_COMMENT;
            } else {
                return ERROR_STATE;
            }
        case MULTI_LINE_COMMENT:
            if (input == '*'){
                return MULTIPLY;
            }
            else if (input != '*'){
                return MULTI_LINE_COMMENT;
            }
            else {
                return ERROR_STATE;
            }
        case ONE_LINE_COMMENT:
            if (input == '\n'){
                return COMMENT_END;
            }
            else {
                return ONE_LINE_COMMENT;
            }
        case MULTIPLY:
            if (input == '/'){
                return COMMENT_END;
            }
            else if(input != '/'){
                return MULTI_LINE_COMMENT;
            }
            else {
                return ERROR_STATE;
            }
        case BACKSLASH:
            if (input == 'n'){
                return COMMENT_END;
            }
            else if(input != 'n'){
                return ONE_LINE_COMMENT;
            }
            else {
                return ERROR_STATE;
            }
        case VAR_PREFIX:
            if (isalpha(input) || input == '_'){
                return VAR_STATE;
            } else {
                return ERROR_STATE;
            }
        case VAR_STATE:
            if (isalpha(input) || input == '_' || isdigit(input)){
                return VAR_STATE;
            } else {
                return ERROR_STATE;
            }
        case REL_LESS_STATE:
            if (input == '='){
                 return REL_LESS_EQ_STATE;
            }
            else if (input == '?'){
                return PROLOG_FIRST;
            }
            else {
                return ERROR_STATE;
            }
        case PROLOG_FIRST:
            if (input == 'p'){
                return PROLOG_SECOND;
            }
            else {
                return ERROR_STATE;
            }
        case PROLOG_SECOND:
            if (input == 'h'){
                return PROLOG_THIRD;
            }
            else {
                return ERROR_STATE;
            }
        case PROLOG_THIRD:
            if (input == 'p'){
                return PROLOG_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case REL_GREATER_STATE:
            if (input == '='){
                return REL_GREATER_EQ_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case QUESTION_MARK:
            if (input == '>'){
                return PROLOG_END_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case FUN_ID_STATE:
            if (isalpha(input) || input == '_' || isdigit(input)){
                return FUN_ID_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case INT_LIT_STATE:
            if (isdigit(input)){
                return INT_LIT_STATE;
            }
            else if (input == 'e' || input == 'E'){
                return INT_LIT_E;
            }
            else if (input == '.'){
                return INT_LIT_DOT;
            } else {
                return ERROR_STATE;
            }
        case INT_LIT_E:
            if (input == '+' || input == '-'){
                return INT_LIT_E_SIGN;
            } else {
                return ERROR_STATE;
            }
        case INT_LIT_E_SIGN:
            if (isdigit(input)){
                return DEC_LIT_E_TMP;
            } else {
                return ERROR_STATE;
            }
        case DEC_LIT_E_TMP:
            if (isdigit(input) || input == '+' || input == '-'){
                return DEC_LIT_E_TMP;
            }
            else if (input == ')' || input == ';' || input == '=' || input == '<' ||
                     input == '>' || input == '*' ||
                     input == '/' || input == ',' || input == '!' || isspace(input)){
                return ERROR_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case INT_LIT_DOT:
            if (isdigit(input)){
                return DEC_LIT_TMP;
            } else {
                ERR_PRINT("INT_LIT_DOT ERROR");
                exit(COMP_ERR_LA);
            }
        case DEC_LIT_TMP:
            if (isdigit(input)){
                return DEC_LIT_TMP;
            }
            else if (input == 'e' || input == 'E'){
                return DEC_LIT_E_TMP;
            }
            else if (input == ')' || input == ';' || input == '=' || input == '<' ||
                     input == '>' || input == '+' || input == '-' || input == '*' ||
                     input == '/' || input == ',' || input == '!' || input == ';' || isspace(input)){
                return DECIMAL_LIT_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case STR_LIT_STATE:
            if (input == '"'){
                return ERROR_STATE;
            } else if(input == '\\'){
                return STR_LIT_ESCAPE;
            } else {
                return STR_LIT_STATE;
            }
        case STR_LIT_ESCAPE:
            if(input == EOF){
                return ERROR_STATE;
            }
            else {
                /* escaping char after \ */
                return STR_LIT_STATE;
            }
        case STR_LIT_BEGIN:
            if (input == EOF) {
                return ERROR_STATE;
            } else {
                return STR_LIT_STATE;
            }
        case ASSIGNMENT_STATE:
            if(input == '='){
                return DOUBLEASSIGNMENT;
            } else {
                return ERROR_STATE;
            }
        case DOUBLEASSIGNMENT:
            if(input == '='){
                return REL_IDENTICAL_STATE;
            } else {
                return ERROR_STATE;
            }
        case EXCLAMATION_MARK:
            if(input == '='){
                return EXCLAMATION_MARK_EQUAL;
            } else {
                return ERROR_STATE;
            }
        case EXCLAMATION_MARK_EQUAL:
            if(input == '='){
                return REL_NEQ_STATE;
            } else {
                return ERROR_STATE;
            }
        case COMMENT_END:
            return ERROR_STATE;
        case SEMICOLON_STATE:
            return ERROR_STATE;
        case R_CURLY_STATE:
            return ERROR_STATE;
        case L_CURLY_STATE:
            return ERROR_STATE;
        case REL_LESS_EQ_STATE:
            return ERROR_STATE;
        case PROLOG_STATE:
            return ERROR_STATE;
        case REL_GREATER_EQ_STATE:
            return ERROR_STATE;
        case PLUS_STATE:
            return ERROR_STATE;
        case MINUS_STATE:
            return ERROR_STATE;
        case ASTERISK_STATE:
            return ERROR_STATE;
        case DOT_STATE:
            return ERROR_STATE;
        case COMMA_STATE:
            return ERROR_STATE;
        case COLON_STATE:
            return ERROR_STATE;
        case L_PAR_STATE:
            return ERROR_STATE;
        case R_PAR_STATE:
            return ERROR_STATE;
        case PROLOG_END_STATE:
            return ERROR_STATE;
        case DECIMAL_LIT_STATE:
            return ERROR_STATE;
        case REL_IDENTICAL_STATE:
            return ERROR_STATE;
        case REL_NEQ_STATE:
            return ERROR_STATE;
        case LEX_EOF_STATE:
            return ERROR_STATE;
    }
    return ERROR_STATE;
}

struct lexeme getToken()
{
    int localOffset = 0; // variable to measure size of string
    state currentState = Start;
    state previousState;
    struct lexeme token;
    int input;
    char buffer[256] = {0};
    int len = 0;

    while (currentState != ERROR_STATE){
        input = getchar();
        previousState = currentState;
        currentState = getNextState(currentState, input);
        /*if we read Function id or variable id we want save it to dynamic string */
        if(currentState == FUN_ID_STATE || currentState == VAR_STATE || currentState == INT_LIT_STATE){
            buffer[len] = input;
            len++;
        }
        if (currentState != MULTI_LINE_COMMENT || currentState != STR_LIT_STATE) {
            if (input == '\n') {
                ctx->current_row++;
            }
        }
        //if we run on EOF state manually setting states
        if(currentState == LEX_EOF_STATE){
          previousState = currentState;
          currentState = ERROR_STATE;
        }
    }
    token.type = makeLexeme(previousState);
    if(previousState == FUN_ID_STATE || previousState == VAR_STATE){
      //calculating id of token
      token.id = strdup(buffer);
      if (!token.id) {
        fprintf(stderr, "Allocation failed\n");
        token.type = ERROR_LEX;
        return token;
      }
    }

    if (previousState == INT_LIT_STATE) {
        token.value.int_val = atoi(buffer);
    }
    if(previousState == FUN_ID_STATE){
      //check if id is not NULL
      if(token.id){
        //call function that checks if FUN_ID is KEYWORD
        token.type = funIdToKeyword(token.id);
      }
    }
    if(previousState != LEX_EOF_STATE && previousState != STR_LIT_STATE){
      ungetc(input, stdin);
    }
    return token;
}
