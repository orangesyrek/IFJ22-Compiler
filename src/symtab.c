#include "symtab.h"



void symtabInit(struct bs_tree *t){
    if(t != NULL){
      t = NULL;
    }
}

int symtabInsert(struct bs_tree *t, char *key, struct bs_data *data) { // 99 for internal error
  if(t != NULL){

      if(strcmp(key, t->key) < 0){
       return symtabInsert(t->left, key, data);
      }
      else if(strcmp(key, t->key) > 0){
        return symtabInsert(t->right, key, data);
      }else{
        free(t->data);
        t->data = data;
        return 0;
    }

  }else{
    struct bs_tree *node = malloc(sizeof(struct bs_tree));
    if(node != NULL){
      node->data = data;
      node->key = key;
      node->left = NULL;
      node->right = NULL;
      t = node;
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

void symtabDispose(struct bs_tree *t){
  if(t != NULL){
    symtabDispose(t->left);
    symtabDispose(t->right);
    free(t);
    t = NULL;
  }
}

int dataInit(struct bs_data *data){
  if(data == NULL){
    struct bs_data *d = malloc(sizeof(struct bs_data));
    if(d != NULL){
      data = d;
      return 0;
    }else{
      return 99;
    }
  }else{
    return 1;
  }
}

void printTree(struct bs_tree *t){
  printf("%s\n", t->key);
  printTree(t->left);
  printTree(t->right);
}
