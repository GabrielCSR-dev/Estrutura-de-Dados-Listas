#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define MAX_LINE 1000
#define MAX_ELEMENTS 1000

typedef struct Node{
  int key;
  struct Node *p;
  struct Node *left;
  struct Node *right;
  int sumleft;
  int sumright;
}Node;

typedef struct MyTree{
  Node * root;
}MyTree;

MyTree *createEmptyTree(){
  MyTree * tree = (MyTree*)malloc(sizeof(MyTree));
  tree->root = NULL;
  return tree;
}

Node *createNode(int key){
  Node *novo = (Node*)malloc(sizeof(Node));
  novo->key = key;
  novo->p = NULL;
  novo->right = NULL;
  novo->sumright = 0;
  novo->left = NULL;
  novo->sumleft =0;
  return novo;
}

int searchNode(Node *n, int key){
  int achou = 0;
  while(n != NULL){
    if(n->key == key){
      achou = 1;
      break;
    }else{
      if(key < n->key){
        n = n->left;
      }else{
        n = n->right;
      }
    }
  }
  return achou;
}

void insertTree(MyTree *t, Node *n){
  if(t->root == NULL){
    t->root = n;
  }else{
    if(!searchNode(t->root, n->key)){
      Node *aux =t->root;
      Node *smart;
      while(aux!=NULL){
        smart = aux;
        if(n->key > aux->key)
          aux = aux->right;
        else
          aux = aux->left;
      }
      if(n->key > smart->key)
        smart->right = n;
      else
        smart->left = n;
      n->p = smart;
    }
  }
}

Node *treeMinimum(Node *n){
  Node *t = n;
  while(t->left != NULL){
    t=t->left;
  }
  return t;
}

Node *sucessor(Node *n){
  Node *suc;
  if(n->right != NULL){
    return treeMinimum(n->right);
  }
  suc = n->p;
  while(suc!= NULL && n == suc->right){
    n = suc;
    suc = suc->p;
  }
  return suc;
}

void burnTree(MyTree *t, Node *n){
  if(n != NULL){
    if(n == t->root){
      t->root = NULL;
    }
    burnTree(t, n->left);
    burnTree(t, n->right);
    free(n);
  }
}

void sumChildren(Node *n){
  if (n->left != NULL) {
    sumChildren(n->left);
    n->sumleft = n->left->key + n->left->sumleft + n->left->sumright;
  }
  if (n->right != NULL) {
    sumChildren(n->right);
    n->sumright = n->right->key + n->right->sumleft + n->right->sumright;
  }
}

void inOrderTreeWalk(Node *n, FILE *out){
  if(n != NULL){
    inOrderTreeWalk(n->left, out);
    if(sucessor(n)!=NULL)
      fprintf(out,"%d (%d) ", n->key, n->sumright - n->sumleft);
    else
      fprintf(out,"%d (%d)\n", n->key, n->sumright - n->sumleft);
    inOrderTreeWalk(n->right, out);
  }
}

void readElements(char line[], Node *n, MyTree *t){
  int key;
  char * token = strtok(line, " ");
  while(token!=NULL){
    key = atoi(token);
    if(key != 0 || strcmp(token,"0") == 0){
      n = createNode(key);
      insertTree(t, n);
    }
    token = strtok(NULL, " ");
  }
}

void readWriteFile(MyTree *tree){
  FILE * input, *output;
  Node *node;
  char line[MAX_LINE];
  
  input = fopen("L2Q2.in","r");
  if(input!=0){
    output = fopen("L2Q2.out", "w");
    if(output == 0){
      perror("Erro! Nao foi possivel abrir o arquivo");
    }else{
      while(!feof(input)){
        if(fgets(line, MAX_LINE, input)!=NULL){
          if(strchr(line,'\n')){
            line[strlen(line)-1]='\0';
            readElements(line, node, tree);
            sumChildren(tree->root);
            inOrderTreeWalk(tree->root, output);
            burnTree(tree, tree->root);
          }
        }
      }
    }
  }else{
    perror("Erro! Nao foi possivel abrir o arquivo");
  }

  fclose(input);
}

int main(){
  MyTree * tree = createEmptyTree();

  readWriteFile(tree);

  free(tree->root);
  free(tree);
  
  return EXIT_SUCCESS;
}