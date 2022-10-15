#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

struct lexeme get_token()
{
    state currentState = Start;
    state previousState;
    struct lexeme token;
    int input;
    while (isspace(input = getchar())); /* we read until there is different char than whitespace */
    while (currentState != ERROR_STATE){
        previousState = currentState;
        currentState = getnextstate(currentState, input);
        if(currentState == FUN_ID_STATE || currentState == VAR_PREFIX){
            /* alokace */
        }
        input = getchar();
    }
    token.type = make_lexeme(previousState);
    return token;
}


static 
lex_types make_lexeme(state final) /* where lexemes are generated, can generate only if you are in state where you can generate something */
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
state getnextstate(state currentState, int input) {  /* decide what is next state based on input and current state */
    if(input == EOF){
        return LEX_EOF_STATE;
    }
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
                return STR_LIT_BEGIN;
            }
            else if (input == '='){ 
                return ASSIGNMENT_STATE;
            }
            else if (input == '!'){ 
                return EXCLAMATION_MARK;
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
            if (input == '\\'){ 
                return BACKSLASH;
            }
            else if(input != '\\'){
                return ONE_LINE_COMMENT;
            }
            else {
                return ERROR_STATE;
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
            if (isdigit(input)){
                return DEC_LIT_E_TMP;
            }
            else if (input == ')' || input == ';' || input == '=' || input == '<' ||
                     input == '>' || input == '+' || input == '-' || input == '*' ||
                     input == '/' || input == ',' || input == '!' || isspace(input)){
                return DECIMAL_LIT_STATE;
            }
            else {
                return ERROR_STATE;
            }
        case INT_LIT_DOT:
            if (isdigit(input)){
                return DEC_LIT_TMP;
            } else {
                return ERROR_STATE;
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
                return DECIMAL_LIT_STATE;
            } 
            else {
                return ERROR_STATE;
            }
        case STR_LIT_BEGIN:
            if (input == '"'){
                return STR_LIT_STATE;
            }
            else if(input != '"' || input != '\\'){
                return STR_LIT_BEGIN;
            }
            else if(input == '\\'){
                return STR_LIT_ESCAPE;
            } else {
                return ERROR_STATE;
            }
        case STR_LIT_ESCAPE:
            if(input == EOF){
                return ERROR_STATE;
            } 
            else {
                return STR_LIT_BEGIN;
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
        case STR_LIT_STATE:
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