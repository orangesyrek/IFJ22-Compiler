#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "lexical_analysis.h"

extern struct compiler_ctx *ctx;

void print_ctx()
{

    printf("%d %d\n", ctx->last_read, ctx->current_row);
    ctx->current_row = 1;
}

void *
la_realloc(void *ptr, size_t size)
{
    void *ret;

    ret = realloc(ptr, size);
    if (!ret) {
        free(ptr);
    }

    return ret;
}


        la_realloc(lex1.id, strlen(lex1.id) + 1);
        lex1.id[strlen(lex1.id)-1] = c;
        }
    }

}

static state = Start;
state getnextstate(state currentState, int input) {
    switch(currentState) {
        case Error:
            fprintf(stderr, "Something is wrong in function getnextstate()\n");
            return (EXIT_FAILURE);
        case Start:
            if (input == '/'){
                return SLASH;
            }
            else if (input == '$'){
                return VAR_PREFIX;
            }
            else if (input == ';'){
                return SEMICOLON;
            }
            else if (input == '}'){
                return R_CURLY;
            }
            else if (input == '{'){
                return L_CURLY;
            }
            else if (input == '<'){
                 return REL_LESS;
            }
            else if (input == '>'){
                return REL_GREATER;
            }
            else if (input == '+'){
                return PLUS;
            }
            else if (input == '-'){
                return MINUS;
            }
            else if (input == '*'){
                return ASTERISK;
            }
            else if (input == '.'){
                return DOT;
            }
            else if (input == ','){
                return COMMA;
            }
            else if (input == ':'){
                return COLON;
            }
            else if (input == '('){
                return L_PAR;
            }
            else if (input == ')'){
                return R_PAR;
            }
            else if (input == '?'){
                return QUESTION_MARK;
            }
            else if (isalpha(input) || input=='_'){
                 return FUN_ID;
            }
            else if (isdigit(input)){
                return INT_LIT;
            }
            else if (input == '"'){
                return STR_LIT_BEGIN;
            }
            else if (input == '='){
                return ASSIGNMENT;
            }
            else if (input == '!'){
                return EXCLAMATION_MARK;
            }
            else if (input == EOF){
                return LEX_EOF;
            }
            else {
                return ERROR;
            }
        case SLASH:
            if (input == '*'){
                return MULTI_LINE_COMMENT;
            }
            else if (input == '/'){
                return ONE_LINE_COMMENT;
            } else {
                return ERROR;
            }
        case MULTI_LINE_COMMENT:
            if (input == '*'){
                return MULTIPLY;
            }
            else if (input != '*'){
                return MULTI_LINE_COMMENT;
            }
            else {
                return ERROR;
            }
        case ONE_LINE_COMMENT:
            if (input == '\\'){
                return BACKSLASH;
            }
            else if(input != '\\'){
                return ONE_LINE_COMMENT;
            }
            else {
                return ERROR;
            }
        case MULTIPLY:
            if (input == '/'){
                return COMMENT_END;
            }
            else if(input != '/'){
                return MULTI_LINE_COMMENT;
            }
            else {
                return ERROR;
            }
        case BACKSLASH:
            if (input == 'n'){
                return COMMENT_END;
            }
            else if(input != 'n'){
                return ONE_LINE_COMMENT;
            }
            else {
                return ERROR;
            }
        case VAR_PREFIX:
            if (isalpha(input) || input == '_'){
                return VAR;
            } else {
                return ERROR;
            }
        case VAR:
            if (isalpha(input) || input == '_' || isdigit(input)){
                return VAR;
            } else {
                return ERROR;
            }
        case REL_LESS
            if (input == '='){
                 return REL_LESS_EQ;
            }
            else if (input == '?'){
                return PROLOG_FIRST;
            }
            else {
                return ERROR;
            }
        case PROLOG_FIRST:
            if (input == 'p'){
                return PROLOG_SECOND;
            }
            else {
                return ERROR;
            }
        case PROLOG_SECOND:
            if (input == 'h'){
                return PROLOG_THIRD;
            }
            else {
                return ERROR;
            }
        case PROLOG_THIRD:
            if (input == 'p'){
                return PROLOG;
            }
            else {
                return ERROR;
            }
        case REL_GREATER:
            if (input == '='){
                return REL_GREATER_EQ;
            }
            else {
                return ERROR;
            }
        case QUESTION_MARK:
            if (input == '>'){
                return PROLOG_END;
            }
            else {
                return ERROR;
            }
        case FUN_ID:
            if (isalpha(input) || input == '_' || isdigit(input)){
                return FUN_ID;
            }
            else {
                return ERROR;
            }
        case INT_LIT:
            if (isdigit(input)){
                return INT_LIT;
            }
            else if (input == 'e' || input == 'E'){
                return INT_LIT_E;
            }
            else if (input == '.'){
                return INT_LIT_DOT;
            } else {
                return ERROR;
            }
        case INT_LIT_E:
            if (input == '+' || input == '-'){
                return INT_LIT_E_SIGN;
            } else {
                return ERROR;
            }
        case INT_LIT_E_SIGN:
            if (isdigit(input)){
                return DEC_LIT_E_TMP;
            } else {
                return ERROR;
            }
        case DEC_LIT_E_TMP:
            if (isdigit(input)){
                return DEC_LIT_E_TMP;
            }
            else if (input == ')' || input == ';' || input == '=' || input == '<' ||
                     input == '>' || input == '+' || input == '-' || input == '*' ||
                     input == '/' || input == ',' || input == '!' || isspace(input)){
                return DECIMAL_LIT;
            }
            else {
                return ERROR;
            }
        case INT_LIT_DOT:
            if (isdigit(input)){
                return DEC_LIT_TMP;
            } else {
                return ERROR;
            }
        case DEC_LIT_TMP:
            if (isdigit(input)){
                return DEC_LIT_TMP;
            }
            else if (input == 'e' || input == 'E'){
                return INT_LIT_E;
            }
            else if (input == ')' || input == ';' || input == '=' || input == '<' ||
                     input == '>' || input == '+' || input == '-' || input == '*' ||
                     input == '/' || input == ',' || input == '!' || isspace(input)){
                return DECIMAL_LIT;
            }
            else {
                return ERROR;
            }
        case STR_LIT_BEGIN:
            if (input == '"'){
                return STR_LIT;
            }
            else if(input != '"' || input != '\\'){
                return STR_LIT_BEGIN;
            }
            else if(input == '\\'){
                return STR_LIT_ESCAPE;
            } else {
                return ERROR;
            }
        case STR_LIT_ESCAPE:
            if(input == EOF){
                return ERROR;
            }
            else {
                return STR_LIT_BEGIN;
            }
        case ASSIGNMENT:
            if(input == '='){
                return DOUBLEASSIGNMENT;
            } else {
                return ERROR;
            }
        case DOUBLEASSIGNMENT:
            if(input == '='){
                return REL_IDENTICAL;
            } else {
                return ERROR;
            }
        case EXCLAMATION_MARK:
            if(input == '='){
                return EXCLAMATION_MARK_EQUAL;
            } else {
                return ERROR;
            }
        case EXCLAMATION_MARK_EQUAL:
            if(input == '='){
                return REL_NEQ;
            } else {
                return ERROR;
            }
    }
}
