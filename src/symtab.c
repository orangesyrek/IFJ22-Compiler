#include "symtab.h"



void symtabInit(struct bs_tree **t){
    if(*t != NULL){
      *t = NULL;
    }
}

int symtabInsert(struct bs_tree **t, const char *key, struct bs_data *data) { // 99 for internal error
  if(*t != NULL){

      if(strcmp(key, (*t)->key) < 0){
       return symtabInsert(&(*t)->left, key, data);
      }
      else if(strcmp(key, (*t)->key) > 0){
        return symtabInsert(&(*t)->right, key, data);
      }else{
        free((*t)->data);
        (*t)->data = data;
        return 0;
    }

  }else{
    struct bs_tree *node = calloc(1, sizeof(struct bs_tree));
    if(node != NULL){
      node->data = data;
      node->key = key;
      node->left = NULL;
      node->right = NULL;
      *t = node;
      return 0;
    }else{
      fprintf(stderr, "%s\n", "Allocation failed");
      return 1;
    }
  }
}

struct bs_data* symtabSearch(struct bs_tree *t, char *key){
  if(t != NULL){
    if(strcmp(key, t->key) < 0){
     return symtabSearch(t->left, key);
    }
    else if(strcmp(key, t->key) > 0){
      return symtabSearch(t->right, key);
    }else{
      return t->data;
  }
  }else{
    return NULL;
  }
}

void symtabDispose(struct bs_tree **t){
  if(*t != NULL){
    symtabDispose(&((*t)->left));
    symtabDispose(&((*t)->right));
    free(*t);
    *t = NULL;
  }
}

int dataInit(struct bs_data **data){
  struct bs_data *d;

  d = calloc(1, sizeof *d);
  if (!d) {
    fprintf(stderr, "Memory allocation error");
    return 99;
  }

  *data = d;
  return 0;
}

void printTree(struct bs_tree *t){
  if (t) {
    printf("node :%s\n", t->key);
    printTree(t->left);
    printTree(t->right);
  }
}

type *
realloc_func_params(type *params, int param_count)
{
  void *ret;

  ret = realloc(params, param_count * sizeof *params);
  if (!ret) {
      free(params);
  }

  return ret;
}

int
insert_builtin_functions(struct bs_tree **tree)
{
  int ret = 0;
  struct bs_data *readi_data, *readf_data, *reads_data, *write_data, *strlen_data;
  struct bs_data *substring_data, *ord_data, *chr_data;

  ret = dataInit(&readi_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&readf_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&reads_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&write_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&strlen_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&substring_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&ord_data);
  if (ret) {
    return 99;
  }

  ret = dataInit(&chr_data);
  if (ret) {
    return 99;
  }

  readi_data->is_function = 1;
  readi_data->data.fdata.return_type = QINT;
  readi_data->data.fdata.is_defined = 1;
  readi_data->data.fdata.params = NULL;
  readi_data->data.fdata.param_count = 0;

  readf_data->is_function = 1;
  readf_data->data.fdata.return_type = QFLOAT;
  readf_data->data.fdata.is_defined = 1;
  readf_data->data.fdata.params = NULL;
  readf_data->data.fdata.param_count = 0;

  reads_data->is_function = 1;
  reads_data->data.fdata.return_type = QSTRING;
  reads_data->data.fdata.is_defined = 1;
  reads_data->data.fdata.params = NULL;
  reads_data->data.fdata.param_count = 0;

  write_data->is_function = 1;
  write_data->data.fdata.return_type = VOID;
  write_data->data.fdata.is_defined = 1;
  write_data->data.fdata.params = NULL;
  write_data->data.fdata.param_count = -1;

  strlen_data->is_function = 1;
  strlen_data->data.fdata.return_type = INT;
  strlen_data->data.fdata.is_defined = 1;
  strlen_data->data.fdata.param_count = 1;
  strlen_data->data.fdata.params = realloc_func_params(strlen_data->data.fdata.params, strlen_data->data.fdata.param_count);
  if (!strlen_data->data.fdata.params) {
    return 99;
  }
  strlen_data->data.fdata.params[0] = STRING;

  substring_data->is_function = 1;
  substring_data->data.fdata.return_type = QSTRING;
  substring_data->data.fdata.is_defined = 1;
  substring_data->data.fdata.param_count = 3;
  substring_data->data.fdata.params = realloc_func_params(substring_data->data.fdata.params, substring_data->data.fdata.param_count);
  if (!substring_data->data.fdata.params) {
    return 99;
  }
  substring_data->data.fdata.params[0] = STRING;
  substring_data->data.fdata.params[1] = INT;
  substring_data->data.fdata.params[2] = INT;

  ord_data->is_function = 1;
  ord_data->data.fdata.return_type = INT;
  ord_data->data.fdata.is_defined = 1;
  ord_data->data.fdata.param_count = 1;
  ord_data->data.fdata.params = realloc_func_params(ord_data->data.fdata.params, ord_data->data.fdata.param_count);
  if (!ord_data->data.fdata.params) {
    return 99;
  }
  ord_data->data.fdata.params[0] = STRING;

  chr_data->is_function = 1;
  chr_data->data.fdata.return_type = STRING;
  chr_data->data.fdata.is_defined = 1;
  chr_data->data.fdata.param_count = 1;
  chr_data->data.fdata.params = realloc_func_params(chr_data->data.fdata.params, chr_data->data.fdata.param_count);
  if (!chr_data->data.fdata.params) {
    return 99;
  }
  chr_data->data.fdata.params[0] = INT;

  ret = symtabInsert(tree, "readi", readi_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "readf", readf_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "reads", reads_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "write", write_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "strlen", strlen_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "substring", substring_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "ord", ord_data);
  if (ret) {
    return 99;
  }

  ret = symtabInsert(tree, "chr", chr_data);
  if (ret) {
    return 99;
  }

  return 0;
}
